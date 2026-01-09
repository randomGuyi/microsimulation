/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#include "msim_component.h"
using namespace core::components;

#include "msim_decoder.h"

msim_decoder::msim_decoder(std::string const & id, std::string const & label)
    : msim_component(id, label){}
