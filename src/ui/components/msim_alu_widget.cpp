#include "msim_alu_widget.h"


msim_alu_widget::msim_alu_widget(msim_alu * alu,
                    shared::svg_loader * loader,
                    QString const & element_id,
                                 QWidget * parent)
        : msim_component_svg_widget{loader, element_id, alu, parent}
{}

void msim_alu_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_alu_widget::show_tooltip(){

}

void msim_alu_widget::hide_tooltip(){

}

