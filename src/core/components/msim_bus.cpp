/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#include "msim_bus.h"
using namespace core::components;
msim_bus::msim_bus(std::string const & id, std::string const & label)
: msim_component{id, label}
, m_value{0}
{}

void msim_bus::set_value(int value){
    m_value = value;
}
int msim_bus::get_value() const{
    return m_value;
}
