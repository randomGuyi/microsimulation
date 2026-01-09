/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#ifndef MICROSIMULATION_MSIM_DECODER_H
#define MICROSIMULATION_MSIM_DECODER_H
#include "msim_component.h"

namespace core::components {
    class msim_decoder final : public msim_component{
    public:
        msim_decoder(std::string const & id, std::string const & label);
        void reset() override{}
    };
}

#endif //MICROSIMULATION_MSIM_DECODER_H