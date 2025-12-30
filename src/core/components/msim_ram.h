#ifndef MSIM_RAM_H
#define MSIM_RAM_H

#include <map>

#include "msim_component.h"
#include "msim_observable_component.h"

namespace core::components {
    class msim_ram : public msim_component,
                     public msim_observable_component<int> {
    public:
        msim_ram(std::string const & id, std::string const & label);

        int get_val_at(int adress);

        void set_val_at(int value, int address);

        [[nodiscard]] const std::map<int, int> & get_ram_data() const ;

    private:
        std::map<int, int> m_ram_data;
        int m_last_address{0};
    };
}
#endif // MSIM_RAM_H
