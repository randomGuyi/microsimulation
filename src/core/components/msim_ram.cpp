#include "msim_ram.h"

msim_ram::msim_ram(std::string const & id, std::string const & label)
    : msim_component{id, label} {
    m_ram_data[0] =  17;
    m_ram_data[1] =  static_cast<int>('k');
    m_ram_data[2] =  static_cast<int>('c');


    m_ram_data[17] = static_cast<int>('m');
    m_ram_data[18] = static_cast<int>('i');
    m_ram_data[19] = static_cast<int>('k');
    m_ram_data[20] = static_cast<int>('r');
    m_ram_data[21] = static_cast<int>('o');
}
