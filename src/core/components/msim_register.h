#ifndef MSIM_REGISTER_H
#define MSIM_REGISTER_H

#include "msim_component.h"
#include "msim_observable_component.h"

namespace core::components {
    class msim_register : public msim_component,
                      public msim_observable_component<int>{
    public:
        msim_register(std::string const & id, std::string const & label);
        void setValue(int value);

        [[nodiscard]]int getValue() const ;
        void reset() override {
            m_value = 0;
            notify(m_value);
        }


    private:
        int m_value;
    };
}

#endif // MSIM_REGISTER_H
