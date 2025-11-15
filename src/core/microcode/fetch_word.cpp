#include "fetch_word.h"

#include <stdexcept>

#define UNUSED(X) (void) X

fetch_word::fetch_word()
    : msim_wrd{}
    , m_mdr_sel{0}
    , m_y_sel  {0}
    , m_x_sel  {0}
{}

void fetch_word::set_mdr_sel(uint8_t mdr_sel_nbl){
    m_mdr_sel = mdr_sel_nbl;
}

void fetch_word::set_y_sel(uint8_t y_sel_nbl){
    /* check if the bit has been already set */
    if((y_sel_nbl & m_y_sel) != 0 ){
        add_error("Y selection was set before");
    }else{
        m_y_sel = y_sel_nbl;
    }
}
void fetch_word::set_x_sel(uint8_t x_sel_nbl){
    /* check if the bit has been already set */
    if((x_sel_nbl & m_x_sel) != 0 ){
        add_error("X selection was set before");
    }else{
        m_x_sel = x_sel_nbl;
    }
}

void fetch_word::set_word(msim_wrd * wrd) {
    fetch_word  * fw  = dynamic_cast<fetch_word *>(wrd);
    if(fw == nullptr) throw std::runtime_error("fetch word can not be set to a different class !");
    m_mdr_sel = fw->m_mdr_sel;
    m_x_sel = fw->m_x_sel;
    m_y_sel = fw->m_y_sel;
}

void fetch_word::set_or(msim_wrd * wrd) {
    fetch_word  * fw  = dynamic_cast<fetch_word *>(wrd);
    if(fw == nullptr) throw std::runtime_error("fetch word can not be set to a different class !");
    m_mdr_sel |= fw->m_mdr_sel;
    m_x_sel |= fw->m_x_sel;
    m_y_sel |= fw->m_y_sel;
}

uint8_t fetch_word::get_mdr_sel(){
    return m_mdr_sel;
}

uint8_t fetch_word::get_y_sel(){
    return m_y_sel;
}

uint8_t fetch_word::get_x_sel(){
    return m_x_sel;
}




