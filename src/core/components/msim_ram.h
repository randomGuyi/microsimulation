#ifndef MSIM_RAM_H
#define MSIM_RAM_H

#include "msim_component.h"


class msim_ram : public msim_component{
public:
    msim_ram(std::string const & id, std::string const & label);
    int get_val_at(int adress){
        (void) adress;
        return 0;
    };

private:

};

#endif // MSIM_RAM_H
