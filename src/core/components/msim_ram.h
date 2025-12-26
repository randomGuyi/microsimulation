#ifndef MSIM_RAM_H
#define MSIM_RAM_H

#include <map>

#include "msim_component.h"


class msim_ram : public msim_component{
public:
    msim_ram(std::string const & id, std::string const & label);
    int get_val_at(int adress){
        if (m_ram_data.find(adress) != m_ram_data.end()){
            return m_ram_data.at(adress);
        }
            return 0;
    }

    void set_val_at(int value, int address) {
        m_ram_data[address] = value;
    }

private:
    std::map<int, int> m_ram_data;
};

#endif // MSIM_RAM_H
