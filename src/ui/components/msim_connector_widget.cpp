#include "msim_connector_widget.h"


msim_connector_widget::msim_connector_widget( msim_connector * con,
                                              shared::svg_loader * loader,
                                   QString const & element_id,
                                   QWidget * parent )
: msim_component_svg_widget{loader, element_id, con, parent}
, m_connections{}
, m_connector{con}
{

    m_connector->subscibe([this](bool new_val){
        QMetaObject::invokeMethod(this, [this, new_val]{
            on_core_value_changed(new_val);
        });
    }) ;
}

void msim_connector_widget::on_core_value_changed(bool value) {
    for (auto & con : m_connections) {
        value ? con->activate() : con->deactivate();
    }
}

void msim_connector_widget::add_line(msim_line_widget * line){
    /* return if the id was already added */
    if (std::find_if(m_connections.begin(), m_connections.end(),
        [=](auto & l) {
         return l->id() == line->id();
        }) != m_connections.end()) {
        return;
    }
    m_connections.append(line);
    if(m_connections.size() > NUMBER_OF_CONNECTIONS ){
        throw std::runtime_error{" to many connectionos for connector: expect " + std::to_string(NUMBER_OF_CONNECTIONS) };
    }
}


void msim_connector_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_connector_widget::show_tooltip(){

}

void msim_connector_widget::hide_tooltip(){

}
