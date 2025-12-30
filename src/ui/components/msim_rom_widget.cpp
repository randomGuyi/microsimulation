
#include "msim_rom_widget.h"

#include "shared/architecture_ids.h"

using namespace gui::components;
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
                            + QPointF{rom_disp_bounds.x() - elem_bounds.x(), 21.0});

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
        bg->setStyleSheet("color: black; background: transparent;");

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
        if (i != 3) {
            bg->show();
            hl->hide();
        }

        /* label for instructions */
        QFont instructionFont;
        instructionFont.setPointSize(2);
        instructionFont.setStretch(72);
        instructionFont.setLetterSpacing(QFont::AbsoluteSpacing, -0.2);

        auto * line_label = new QLabel(bg);
        line_label->setFont(instructionFont);
        line_label->setAlignment(Qt::AlignCenter);
        line_label->setStyleSheet("color: black; background: transparent;");
        line_label->setGeometry(0, 0, bg->width(), bg->height  ());
        line_label->setText("0000 000 000 0000 00000 00 00 0000000"); /* dummy instruction */

        m_instruction_lines.push_back({hl, line_label} );
    }
    m_rom->subscibe([this](int new_line){
        QMetaObject::invokeMethod(this, [this, new_line]{
            on_core_value_changed(new_line);
        });
    }) ;
}

void msim_rom_widget::on_core_value_changed(int line_number) {
}

void msim_rom_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_rom_widget::show_tooltip(){

}

void msim_rom_widget::hide_tooltip(){

}
