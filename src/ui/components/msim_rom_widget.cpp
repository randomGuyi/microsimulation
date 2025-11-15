
#include "msim_rom_widget.h"


msim_rom_widget::msim_rom_widget(msim_rom * rom,
                                 shared::svg_loader * loader,
                    QString const & element_id,
                                 QWidget * parent)
        : msim_component_svg_widget{loader, element_id, rom, parent}
{}

void msim_rom_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_rom_widget::show_tooltip(){

}

void msim_rom_widget::hide_tooltip(){

}
