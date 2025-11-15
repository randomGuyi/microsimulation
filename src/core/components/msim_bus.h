#ifndef MSIM_BUS_H
#define MSIM_BUS_H

#include "core/components/msim_component.h"

class msim_bus : public msim_component{
public:
    msim_bus(std::string const & id, std::string const & label);
    void set_value(int value);
    int get_value() const;

private:
    int m_value;
};

#endif // MSIM_BUS_H
