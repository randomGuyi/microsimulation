#include "msim_bit.h"
#include <iostream>

using namespace core::components;

msim_bit::msim_bit(std::string const & id, std::string const & label, bool value)
    : msim_component{id, label}
    , m_value{value}
{}

bool msim_bit::value() const{
    return m_value;
}

void msim_bit::set_value(bool val) {
    if(m_value != val){
        m_value = val;
        notify(m_value);
        std::cout << "core changed" << std::endl;
    }
}

