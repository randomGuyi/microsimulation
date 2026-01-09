/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#include "msim_cop.h"

using namespace core::components;

msim_cop::msim_cop(std::string const & id, std::string const & label)
    : msim_register{id, label}
    , m_mask{0}
    , m_cn{0}
{}

void msim_cop::set_mask(int msk){
    m_mask = msk;
}

void msim_cop::set_cn(int cn){
    m_cn = cn;
}
