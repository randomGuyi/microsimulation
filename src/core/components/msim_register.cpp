#include "msim_register.h"

using namespace core::components;

msim_register::msim_register(std::string const & id, std::string const & label)
    : msim_component{id, label}
    , m_value{0}
{}

void msim_register::setValue(int value) {
    m_value = value;
    notify(value);
}

int msim_register::getValue() const{
    return m_value;
}
