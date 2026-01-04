
#include "msim_cop_widget.h"

#include <QToolTip>

using namespace gui::components;
msim_cop_widget::msim_cop_widget(core::components::msim_cop * cop,
                                 shared::svg_loader * loader,
                                 QString const & element_id,
                                 QWidget * parent)
        : msim_component_svg_widget{loader, element_id, cop, parent},
m_cop{cop}{

    /* tooltip */
    QFont tooltip_font{};
    tooltip_font.setPointSize(14);
    QToolTip::setFont(tooltip_font);
}

void msim_cop_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_cop_widget::show_tooltip(){
    QString tooltip_text = "Cop-Value:" +  QString::number(m_cop->getValue());
    QToolTip::showText(mapToGlobal(QPoint{width()/2, 0}), tooltip_text, this);
}

void msim_cop_widget::hide_tooltip(){
    QToolTip::hideText();

}
