/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#include "msim_bus_widget.h"

#include <QToolTip>

using namespace gui::components;

msim_bus_widget::msim_bus_widget(core::components::msim_bus * bus,
                                 shared::svg_loader * loader,
                                 QString const & element_id,
                                 QWidget * parent)
:msim_component_svg_widget{loader, element_id, bus, parent}
, m_bus{bus}{
    /* tooltip */
    QFont tooltip_font{};
    tooltip_font.setPointSize(14);
    QToolTip::setFont(tooltip_font);
}

void msim_bus_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_bus_widget::show_tooltip(){
    QString bus_value = "Value:" + QString::number(m_bus->get_value());
    QToolTip::showText(mapToGlobal(QPoint{width()/2, 0}), bus_value, this);

}

void msim_bus_widget::hide_tooltip(){
    QToolTip::hideText();
}
