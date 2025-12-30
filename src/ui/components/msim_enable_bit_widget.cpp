#include "msim_enable_bit_widget.h"


using namespace gui::components;
msim_enable_bit_widget::msim_enable_bit_widget(core::components::msim_enable_bit * bit,
                                               shared::svg_loader * loader,
                                               QString const & element_id,
                                               QWidget * parent)
    : msim_bit_widget{bit, loader, element_id, parent}
    , m_bit{bit}
{}

void msim_enable_bit_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_enable_bit_widget::show_tooltip(){

}

void msim_enable_bit_widget::hide_tooltip(){

}
