//
// Created by Arthur on 19.11.25.
//

#ifndef MICROSIMULATION_MSIM_DECODER_H
#define MICROSIMULATION_MSIM_DECODER_H
#include "msim_component.h"


class msim_decoder : public msim_component{
public:
    msim_decoder(std::string const & id, std::string const & label);
};


#endif //MICROSIMULATION_MSIM_DECODER_H