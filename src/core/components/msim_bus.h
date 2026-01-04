#ifndef MSIM_BUS_H
#define MSIM_BUS_H

#include "msim_alu.h"
#include "core/components/msim_component.h"

namespace core::components {
    class msim_bus : public msim_component{
    public:
        msim_bus(std::string const & id, std::string const & label);
        void set_value(int value);
        int get_value() const;
        void reset() override {
            m_value = 0;
        }

    private:
        int m_value;
    };
}

#endif // MSIM_BUS_H
