#include "details_controller.h"
#include "../views/details_view.h"

namespace gui::views {

details_controller & details_controller::instance(){
    static details_controller inst;
    return inst;
}

details_controller::details_controller() = default;

void details_controller::set_details_view(gui::views::details_view * view){
    m_view = view;
}

void details_controller::on_ram_clicked(core::components::msim_ram * ram){
    if(!m_view) return;
    m_view->show_ram_details(ram);
}

void details_controller::on_rom_clicked(core::components::msim_rom * rom){
    if(!m_view) return;
    m_view->show_rom_details(rom);
}

} // namespace ui_controller
