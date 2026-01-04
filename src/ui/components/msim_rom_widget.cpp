#include "msim_rom_widget.h"

#include "shared/architecture_ids.h"
#include <sstream>
#include <algorithm>
#include <QSizePolicy>
#include <QVBoxLayout>
#include "ui/controller/details_controller.h"

using namespace gui::components;
using gui::views::details_controller;
msim_rom_widget::msim_rom_widget(core::components::msim_rom * rom,
                                 shared::svg_loader * loader,
                                 QString const & element_id,
                                 QWidget * parent)
         : msim_component_svg_widget{loader, element_id, rom, parent}
 , m_rom{rom}
{
    QRectF elem_bounds = m_loader->get_renderer().boundsOnElement(element_id);
    QRectF rom_disp_bounds = m_loader->get_instance().get_renderer().boundsOnElement(ID_DISP_ROM);

    QLabel * rom_label = new QLabel(this);
    rom_label->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    rom_label->setText("   ROM");
    /* translate to widget-local coordinates */
    rom_disp_bounds.translate(-rom_disp_bounds.topLeft()
                            + QPointF{rom_disp_bounds.x() - elem_bounds.x() + 14, 21.0});

    int rows = 6;
    m_instruction_lines.reserve(rows);

    for (int i =0; i < rows; ++i) {
        QRectF line_rect(
            rom_disp_bounds.left(),
            rom_disp_bounds.top() + i * (rom_disp_bounds.height() / rows),
            rom_disp_bounds.width(),
            rom_disp_bounds.height() / rows
        );
        auto * bg = new QLabel(this);
        /* normal background */
        bg->setGeometry(line_rect.toRect());
        // visible but subtle background for non-selected rows
        bg->setStyleSheet("color: black; background: transparent;");//rgba(240,240,240,170);");

        /* highlight background */

        auto * hl = new QLabel(bg);
        int insetX = int(bg->width()  * 0.02);
        int insetY = int(bg->height() * 0.16);

        hl->setGeometry(
            insetX,
            insetY,
            bg->width()  - 2 * insetX,
            bg->height() - 2 * insetY
        );

        hl->setStyleSheet(
          "background: rgba(255,220,0,200);"
            "border: 1px solid rgba(200,180,0,220);"
            "border-radius: 2px;"
        );
        bg->show();
        hl->hide();

        /* label for instructions */
        QFont instructionFont;
        instructionFont.setPointSize(2);
        instructionFont.setStretch(72);
        instructionFont.setLetterSpacing(QFont::AbsoluteSpacing, -0.2);

        auto * line_label = new QLabel(bg);
        line_label->setFont(instructionFont);
        line_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        line_label->setStyleSheet("color: black; background: transparent;");
        line_label->setContentsMargins(0, 0, 0, 0);
        line_label->setMargin(0);
        line_label->setWordWrap(false);
        line_label->setTextFormat(Qt::PlainText);
        // Use a vertical layout with stretches to guarantee vertical centering
        auto * vlayout = new QVBoxLayout(bg);
        vlayout->setContentsMargins(0, 0, 0, 0);
        vlayout->setSpacing(0);
        vlayout->addStretch(1);
        // Fix label height to font line-height so it doesn't expand vertically
        QFontMetrics fm(instructionFont);
        int fixedLabelH = fm.height() + 2; // small padding
        // make the label strictly fixed height so the layout centers it vertically
        line_label->setFixedHeight(fixedLabelH);
        line_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        vlayout->addWidget(line_label, 0, Qt::AlignHCenter);
        vlayout->addStretch(1);


         m_instruction_lines.push_back({hl, line_label} );
     }
    m_rom->subscibe([this](int new_line){
        QMetaObject::invokeMethod(this, [this, new_line]{
            on_core_value_changed(new_line);
        });
    }) ;

    // connect clicks to details controller
    connect(this, &msim_rom_widget::clicked, &details_controller::instance(), &details_controller::on_rom_clicked);
}

void msim_rom_widget::mousePressEvent(QMouseEvent * event){
    emit clicked(m_rom);
    msim_component_svg_widget::mousePressEvent(event);
}

void msim_rom_widget::on_core_value_changed(int line_number) {
    auto const & instructions = m_rom->get_all_instructions();

    const int N = static_cast<int>(instructions.size());
    const int rows = static_cast<int>(m_instruction_lines.size());
    if (rows == 0) return;

    /* If there are no instructions, clear the display and hide highlights. */
    if (N == 0) {
        for (auto &hl_pair : m_instruction_lines) {
            if (hl_pair.text) {
                hl_pair.text->setText("");
                if (QWidget *base = hl_pair.text->parentWidget()) base->show();
            }
            if (hl_pair.bg) hl_pair.bg->hide();
        }
        m_old_line = 0;
        return;
    }

    /* Clamp requested line to valid range */
    int new_line = std::max(0, std::min(line_number, N - 1));

    /* Choose a sliding window of size `rows` into the instruction vector so new_line
     is visible and (if possible) not positioned at the first or last slot. */
    int start = new_line - rows / 2; // try to center
    if (start < 0) start = 0;
    if (start > N - rows) start = std::max(0, N - rows);

    int pos = new_line - start;
    if (pos == 0 && start > 0) {
        start = std::max(0, start - 1);
    } else if (pos == rows - 1 && start < std::max(0, N - rows)) {
        start = std::min(std::max(0, N - rows), start + 1);
    }

    for (int i = 0; i < rows; ++i) {
        int idx = start + i;
        auto &hl_pair = m_instruction_lines[i];
        QLabel *highlight = hl_pair.bg; // note: stored "bg" is actually the highlight QLabel
        QLabel *text = hl_pair.text;

        if (idx >= 0 && idx < N) {

            std::ostringstream oss;
            if (instructions[idx]) {
                oss << *instructions[idx];
            }
            QString s = QString::fromStdString(oss.str()).trimmed();
            if (s.isEmpty()) s = "";

            if (text) {
                text->setTextFormat(Qt::PlainText);
                text->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                text->setText(s);
                text->update();
            }

             if (highlight) {
                 if (idx == new_line) highlight->show();
                 else highlight->hide();
             }
        } else {
            // Out of range: clear
            if (text) text->setText("");
            if (highlight) highlight->hide();
        }
    }

    m_old_line = new_line;
}

void msim_rom_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_rom_widget::show_tooltip(){

}

void msim_rom_widget::hide_tooltip(){

}
