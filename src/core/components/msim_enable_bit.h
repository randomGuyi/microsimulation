/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MSIM_ENABLE_BIT_H
#define MSIM_ENABLE_BIT_H

#include "msim_bit.h"

namespace core::components {
    class msim_enable_bit : public msim_bit
    {
    public:
        msim_enable_bit(std::string const & id, std::string const & label);
    };
}
#endif // MSIM_ENABLE_BIT_H
