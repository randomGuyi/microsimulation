#ifndef MSIM_BIT_H
#define MSIM_BIT_H

#include "core/components/msim_component.h"
#include "core/components/msim_observable_component.h"
#include <string>


class msim_bit : public msim_component,
                 public msim_observable_component<bool>
{
public:
    msim_bit(std::string const & id, std::string const & label, bool value = false);
    bool value() const;
    void set_value(bool val) ;

private:
    bool m_value;
};

#endif // MSIM_BIT_H
