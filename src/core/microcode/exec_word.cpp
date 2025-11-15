#include "exec_word.h"

#include <stdexcept>


exec_word::exec_word()
    : msim_wrd{}
    , m_nibble{0}
{}

void exec_word::set_nibble(uint8_t nbl){
    m_nibble = nbl;
}

void exec_word::set_or_nibble(uint8_t nbl){
    m_nibble |= nbl;
}

uint8_t exec_word::get_nibble(){
    return m_nibble;
}
void exec_word::set_word(msim_wrd * wrd) {
    exec_word * other = dynamic_cast<exec_word*>(wrd);
    if(other == nullptr) throw std::runtime_error("exec_word can not be set to a different class !");
    set_nibble(other->m_nibble);
}

void exec_word::set_or(msim_wrd * wrd) {
    exec_word * other = dynamic_cast<exec_word*>(wrd);
    if(other == nullptr) throw std::runtime_error("exec_word can not be set to a different class !");
    set_or_nibble(other->m_nibble);
}
