#include "msim_component.h"

using namespace core::components;

msim_component::msim_component(std::string const & id, std::string const & label)
    : m_id{id}
    , m_label{label}
{}

std::string msim_component::id() const{
    return m_id;
}
std::string msim_component::label() const{
    return m_label;
}
