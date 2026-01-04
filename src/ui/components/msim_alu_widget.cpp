#include "msim_alu_widget.h"

#include <QLabel>
#include <QToolTip>

#include "shared/architecture_ids.h"

using namespace gui::components;
msim_alu_widget::msim_alu_widget(core::components::msim_alu * alu,
                                 shared::svg_loader * loader,
                                 QString const & element_id,
                                 QWidget * parent)
: msim_component_svg_widget{loader, element_id, alu, parent}
, m_alu{alu}
, m_operation_label{nullptr}
, m_lu_label{nullptr}
, m_au_label{nullptr}
, m_mux_label{nullptr}
{

    /* operatrion Display */
    QRectF op_disp_rect = m_loader->get_renderer().boundsOnElement(ID_FRAMEOP_ALU);
    /* Logical Unit Display */
    QRectF lu_disp_rect = m_loader->get_renderer().boundsOnElement(ID_FRAMELU_ALU);
    QRectF lu_label_rect = m_loader->get_renderer().boundsOnElement(ID_LABEFRAMELU_ALU);

    /* Arithmetic Unit Display */
    QRectF au_disp_rect = m_loader->get_renderer().boundsOnElement(ID_FRAMEAU_ALU);
    QRectF au_label_rect = m_loader->get_renderer().boundsOnElement(ID_FRAMELABELAU_ALU);

    /* Mux Display */
    QRectF mux_disp_rect = m_loader->get_renderer().boundsOnElement(ID_FRAMEMUX_ALU);
    QRectF mux_label_rect = m_loader->get_renderer().boundsOnElement(ID_FRAMELABELMUL_ALU);

    /* get element bounds and convert to widget-local coords */
    QRectF elem_bounds = m_loader->get_renderer().boundsOnElement(element_id);

    /* translate all rects to widget-local coordinates */


    op_disp_rect.translate(- op_disp_rect.topLeft() + QPointF{op_disp_rect.x() - elem_bounds.x() + 14.0, 27});

    lu_label_rect.translate(- lu_label_rect.topLeft() + QPointF{lu_label_rect.x() - elem_bounds.x() + 14.0, 9.45});
    lu_disp_rect.translate(- lu_disp_rect.topLeft() + QPointF{lu_disp_rect.x() - elem_bounds.x() + 14.0, 14});

    au_label_rect.translate(- au_label_rect.topLeft() + QPointF{au_label_rect.x() - elem_bounds.x() + 14.0, 9.45});
    au_disp_rect.translate(- au_disp_rect.topLeft() + QPointF{au_disp_rect.x() - elem_bounds.x() + 14.0, 14});

    mux_label_rect.translate(- mux_label_rect.topLeft() + QPointF{mux_label_rect.x() - elem_bounds.x() + 14.0, 38.45});
    mux_disp_rect.translate(- mux_disp_rect.topLeft() + QPointF{mux_disp_rect.x() - elem_bounds.x() + 14.0, 41.8});


    /* --- Operation display (centered big symbol) --- */
    {
        auto *bg = new QLabel(this);
        bg->setGeometry(op_disp_rect.toRect());
        bg->setStyleSheet("background: transparent;");

        QFont opFont;
        opFont.setPointSize(4);

        m_operation_label = new QLabel(bg);
        m_operation_label->setAlignment(Qt::AlignCenter);
        m_operation_label->setFont(opFont);
        m_operation_label->setStyleSheet("color: black; background: transparent; font-weight: bold;");
        m_operation_label->setGeometry(0, 0, bg->width(), bg->height());

        m_operation_label->setText("."); /* dummy operation */
    }


    auto makeDisplay = [&](QRectF dispRect, QRectF nameRect,
                           QString name, HighlightLabel &out)
    {
        auto *bg = new QLabel(this);
        bg->setGeometry(dispRect.toRect());
        bg->setStyleSheet("color: black; background: transparent;");

        int val_h = int(bg->height() * 0.70);

        /* highlight background */
        auto *hl = new QLabel(bg);
        int insetX = int(bg->width()  * 0.06);
        int insetY = int(val_h * 0.16);

        hl->setGeometry(
            insetX,
            insetY,
            bg->width()  - 2 * insetX,
            val_h        - 2 * insetY
        );
        hl->setStyleSheet(
          "background: rgba(255,220,0,200);"
            "border: 1px solid rgba(200,180,0,220);"
            "border-radius: 2px;"
        );
        hl->hide();

        /* value label (Text) */
        QFont valueFont;
        valueFont.setPointSize(6);
        auto *val = new QLabel(bg);
        val->setFont(valueFont);
        val->setAlignment(Qt::AlignCenter);
        val->setStyleSheet("color: black; background: transparent;");
        val->setGeometry(0, 0, bg->width(), val_h);
        val->setText(".");

        out.bg   = hl;
        out.text = val;

        QFont labelFont;
        labelFont.setPointSize(4);
        /* name label unverändert */
        auto *nameLbl = new QLabel(this);
        nameLbl->setFont(labelFont);
        nameLbl->setGeometry(nameRect.toRect());
        nameLbl->setAlignment(Qt::AlignCenter);
        nameLbl->setText(name);
    };

    /* create AU, LU and MUX displays with dummy values */
    makeDisplay(au_disp_rect, au_label_rect, QStringLiteral("AU"), m_au_label);       /* arithmetic result */
    makeDisplay(lu_disp_rect, lu_label_rect, QStringLiteral("LU"), m_lu_label);      /* logical result */
    makeDisplay(mux_disp_rect, mux_label_rect, QStringLiteral("MUX"), m_mux_label);   /* mux selection / result */

    /* setup ALU subscription */

    m_alu->subscibe([this](std::pair<core::components::mux_selection, int> new_result){
        QMetaObject::invokeMethod(this, [this, new_result]{
            on_core_value_changed(new_result);
        });
    }) ;

    /* tooltip */
    QFont tooltip_font{};
    tooltip_font.setPointSize(14);
    QToolTip::setFont(tooltip_font);

}

void msim_alu_widget::on_core_value_changed(std::pair<core::components::mux_selection, int> new_result) {
    auto activate = [](HighlightLabel &on, HighlightLabel &off, int value) {
        off.bg->hide();
        on.bg->show();
        on.text->setText(QString::number(value));
    };
    switch (new_result.first) {
        case core::components::mux_selection::AU_RESULT:
            activate(m_au_label, m_lu_label, new_result.second);
            m_mux_label.text->setText("AU: " + QString::number(new_result.second));
            m_mux_label.bg->hide();
            break;

        case core::components::mux_selection::LU_RESULT:
            activate(m_lu_label, m_au_label, new_result.second);
            m_mux_label.text->setText("LU: " + QString::number(new_result.second));
            m_mux_label.bg->hide();
            break;

        case core::components::mux_selection::CONSTANT: {
            m_au_label.bg->hide();
            m_lu_label.bg->hide();
            m_mux_label.bg->show();
            m_mux_label.text->setText(QString::number(new_result.second));
            break;
        }
    }

    m_operation_label->setText(getOperation());

}

QString msim_alu_widget::getOperation() {
    switch (m_alu->get_operation()) {
        case Z_Z:            return "Z";
        case Z_X:           return "X";
        case Z_Y:           return "Y";
        case Z_CONST:       return "C";
        case Z_X_PLS_Y:    return "X+Y";
        case Z_INC_X:      return "X+1";
        case Z_INC_Y:      return "Y+1";
        case Z_X_TIMES_Y:  return "X*Y";
        case Z_X_MINUS_Y:  return "X-Y";
        case Z_DEC_X:      return "X-1";
        case Z_DEC_Y:      return "Y-1";
        case Z_X_DIV_Y:    return "X/Y";
        case Z_X_SHL_Y:    return "X<<Y";
        case Z_MINUS_X:    return "-X";
        default:            return "UNK";
    }
}

void msim_alu_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_alu_widget::show_tooltip(){
    QString mux_selection{};
    switch(m_alu->get_mux_selection()){
        case core::components::mux_selection::AU_RESULT: mux_selection = "Au-Result"; break;
        case core::components::mux_selection::LU_RESULT: mux_selection = "Lu-Result"; break;
        case core::components::mux_selection::CONSTANT:   mux_selection = "Constant";  break;
    }
    QString alu_flags = QString{"Flags: %1%2%3%4"}
        .arg((m_alu->get_flags() & 0b00001000) ? "Z" : "0")
        .arg((m_alu->get_flags() & 0b00000100) ? "+" : "0")
        .arg((m_alu->get_flags() & 0b00000010) ? "-" : "0")
        .arg((m_alu->get_flags() & 0b00000001) ? "Of" : "0");

    QString operation_str = getOperation();

    QString result_str = QString::number(m_alu->get_result());


    QString tooltip = QString{"Mux Selection: %1\nFlags: %2\nOperation: %3\nResult: %4\n "}
    .arg(mux_selection)
    .arg(alu_flags)
    .arg(operation_str)
    .arg(result_str);
    QToolTip::showText(cursor().pos(), tooltip, this, QRect(), 500000); /* show tooltip for 5 sec */
}

void msim_alu_widget::hide_tooltip(){
    QToolTip::hideText();
}

