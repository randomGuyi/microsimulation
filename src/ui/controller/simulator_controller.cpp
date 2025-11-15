#include "simulator_controller.h"


simulator_controller::simulator_controller(){

}

void simulator_controller::register_drop_target(drop_target * target){
//    connect(target, &drop_target::component_dropped,
 //           this, &simulator_controller::handle_component_drop);

}
void simulator_controller::handle_component_drop(QString const & id, QString const & label){
  //  if(! core_is_valid_drop(id)) return;

   // auto core_component = core_create_component(id);

    //auto proxy = ui_create_proxy_for(core_component, label);

    //scene->addItem(proxy);

}
