#include "msim_ram.h"

msim_ram::msim_ram(std::string const & id, std::string const & label)
    : msim_component{id, label} {}

int msim_ram::get_val_at(int adress){
    if (m_ram_data.find(adress) != m_ram_data.end()){
        return m_ram_data.at(adress);
    }
    return 0;
}

void msim_ram::set_val_at(int value, int address) {
    m_ram_data[address] = value;
    notify(address);
}

[[nodiscard]] const std::map<int, int> & msim_ram::get_ram_data() const {
    return m_ram_data;
}
