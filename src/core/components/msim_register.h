#ifndef MSIM_REGISTER_H
#define MSIM_REGISTER_H

#include "msim_component.h"
#include "msim_observable_component.h"


class msim_register : public msim_component,
                      public msim_observable_component<int>{
public:
    msim_register(std::string const & id, std::string const & label);
    void setValue(uint32_t value);
    uint32_t getValue() const ;


private:
    uint32_t m_value;
};

#endif // MSIM_REGISTER_H
