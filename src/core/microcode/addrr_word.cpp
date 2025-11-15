#include "addrr_word.h"

#include <stdexcept>
#define UNUSED(x) (void)(x)

#define DEFAULT_CN -1
#define DEFAULT_MASK 0

addrr_word::addrr_word()
    : msim_wrd{}
    , m_cn{DEFAULT_CN}
    , m_mask{DEFAULT_MASK}
{}

bool addrr_word::is_defalult(){
    return m_cn == DEFAULT_CN && m_mask == DEFAULT_MASK;
}

int addrr_word::get_cn(){
    return m_cn;
}

int addrr_word::get_mask(){
    return m_mask;
}

void addrr_word::set_default(){
    m_cn = DEFAULT_MASK;
    m_mask = DEFAULT_MASK;
}

void addrr_word::set_cn(int cn ){
    if(m_cn < -1){
        add_error("unable to set CN to invalid value :" + std::to_string(cn));
        return;
    }
    m_cn = cn;
}
void addrr_word::set_mask(uint8_t mask ){
    m_mask = mask;
}


void addrr_word::set_word(msim_wrd * wrd) {
    addrr_word * other = dynamic_cast<addrr_word *>(wrd);
    if(other == nullptr) throw std::runtime_error("addrr_word can not be set to a different class !");
    m_cn = other->m_cn;
    m_mask = other->m_mask;
}

void addrr_word::set_or(msim_wrd * wrd) {
    addrr_word * other = dynamic_cast<addrr_word *>(wrd);
    if(other == nullptr) throw std::runtime_error("addrr_word can not be set to a different class !");
    if(m_cn == DEFAULT_CN){
        m_cn = other->m_cn;
    }
    m_mask |= other->m_mask;
}
