/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#include "msim_connector.h"

#include "msim_observable_component.h"

using namespace core::components;
msim_connector::msim_connector(std::string const & id, std::string const & label)
    : msim_component{id, label}
    , m_enabled{false}
{}

bool msim_connector::is_enabled() const {
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
