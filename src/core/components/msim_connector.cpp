#include "msim_connector.h"

#include "msim_observable_component.h"


msim_connector::msim_connector(std::string const & id, std::string const & label)
    : msim_component{id, label}
    , m_enabled{false}
{}

bool msim_connector::is_enabled(){
    return m_enabled;
}

void msim_connector::enable(){
    m_enabled= true;
    notify(true);

}

void msim_connector::disable(){
    m_enabled = false;
    notify(false);
}
