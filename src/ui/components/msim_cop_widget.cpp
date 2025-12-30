
#include "msim_cop_widget.h"

using namespace gui::components;
msim_cop_widget::msim_cop_widget(core::components::msim_cop * cop,
                                 shared::svg_loader * loader,
                                 QString const & element_id,
                                 QWidget * parent)
        : msim_component_svg_widget{loader, element_id, cop, parent}
{}

void msim_cop_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_cop_widget::show_tooltip(){

}

void msim_cop_widget::hide_tooltip(){

}
