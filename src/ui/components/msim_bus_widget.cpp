#include "msim_bus_widget.h"



msim_bus_widget::msim_bus_widget(msim_bus * bus,
                    shared::svg_loader * loader,
                    QString const & element_id,
                    QWidget * parent)
:msim_component_svg_widget{loader, element_id, bus, parent}
{}

void msim_bus_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_bus_widget::show_tooltip(){

}

void msim_bus_widget::hide_tooltip(){

}
