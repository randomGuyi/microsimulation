#include "msim_register.h"

msim_register::msim_register(std::string const & id, std::string const & label)
    : msim_component{id, label}
    , m_value{0}
{}

void msim_register::setValue(uint32_t value) {
    m_value = value;
    notify(value);
}

uint32_t msim_register::getValue() const{
    return m_value;
}
