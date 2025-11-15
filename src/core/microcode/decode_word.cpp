#include "decode_word.h"
#define UNUSED(x) (void)(x)

#define UNINITIALIZED_CONST -1

decode_word::decode_word()
    :msim_wrd{}
    , m_constant{UNINITIALIZED_CONST}
    , m_operation{Z_Z}
{}

void decode_word::set_const_number(int const_n){
    if(m_constant != UNINITIALIZED_CONST){
        add_error("decode: constant number was set twice");
        return;
    }
    m_constant = const_n;
}

int decode_word::get_const_number(){
    return m_constant;
}

void decode_word::set_default(){
    m_constant = UNINITIALIZED_CONST;
    m_operation = Z_Z;
}

void decode_word::set_operation(uint8_t operation){
    m_operation = operation;
}

uint8_t decode_word::get_operation(){
    return m_operation;
}


void decode_word::set_word(msim_wrd * wrd) {
    decode_word* other = dynamic_cast<decode_word*>(wrd);
    if(other == nullptr) throw std::runtime_error("decode_word can not be set to a different class !");
    m_constant = other->m_constant;
    m_operation = other->m_operation;
}

void decode_word::set_or(msim_wrd * wrd) {
    decode_word* other = dynamic_cast<decode_word*>(wrd);
    if(other == nullptr) throw std::runtime_error("decode_word can not be set to a different class !");
    m_constant = other->m_constant;
    m_operation = other->m_operation;
}
