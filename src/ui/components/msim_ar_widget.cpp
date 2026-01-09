/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#include "msim_ar_widget.h"
using namespace gui::components;
msim_ar_widget::msim_ar_widget(core::components::msim_ar * ar,
                               shared::svg_loader * loader,
                    QString const & element_id,
                                 QWidget * parent)
        : msim_component_svg_widget{loader, element_id, ar, parent}
{}

void msim_ar_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_ar_widget::show_tooltip(){

}

void msim_ar_widget::hide_tooltip(){

}
