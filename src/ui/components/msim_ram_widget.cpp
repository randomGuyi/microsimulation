

#include "msim_ram_widget.h"


msim_ram_widget::msim_ram_widget(msim_ram * ram,
                                 shared::svg_loader * loader,
                    QString const & element_id,
                                 QWidget * parent)
        : msim_component_svg_widget{loader, element_id, ram, parent}
{}

void msim_ram_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_ram_widget::show_tooltip(){

}

void msim_ram_widget::hide_tooltip(){

}
