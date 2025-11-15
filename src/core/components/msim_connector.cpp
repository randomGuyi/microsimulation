#include "msim_connector.h"


msim_connector::msim_connector(std::string const & id, std::string const & label)
    : msim_component{id, label}
    , m_enabled{false}
{}

bool msim_connector::is_enabled(){
    return m_enabled;
}

void msim_connector::enable(){
    m_enabled= true;
}

void msim_connector::disable(){
    m_enabled = false;
}
