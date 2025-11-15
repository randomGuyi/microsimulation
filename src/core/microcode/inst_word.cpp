#include "inst_word.h"

#define UNUSED(X) (void) X

inst_word::inst_word()
    : msim_wrd{}
    , m_ram_mode{ram_mode::WAIT}
{}

void inst_word::set_read(bool read){
    if(read){
        m_ram_mode = ram_mode::READ;
    }
}

void inst_word::set_fetch_ops(fetch_word & fw){
    if(m_fetch_wrd != nullptr){
        add_error("fetch options for instruction word already set!");
        return;
    }
    m_fetch_wrd = std::make_unique<fetch_word>(fw);
}

void inst_word::set_decode_ops(decode_word & dw){
    if(m_decode_wrd != nullptr){
        add_error("decode options for instruction word already set!");
        return;
    }
    m_decode_wrd = std::make_unique<decode_word>(dw);
}

void inst_word::set_ar_ops(addrr_word & aw){
    if(m_addrr_wrd != nullptr){
        add_error("addrr options for instruction word already set!");
        return;
    }
    m_addrr_wrd = std::make_unique<addrr_word>(aw);
}

void inst_word::set_exec_ops(exec_word & ew){
    if(m_exec_wrd != nullptr){
        add_error("exec options for instruction word already set!");
        return;
    }
    m_exec_wrd = std::make_unique<exec_word>(ew);
}

void inst_word::set_write(bool write){
    if(write){
        m_ram_mode = ram_mode::WRITE;
    }
}

bool inst_word::get_read()const{
    return m_ram_mode == ram_mode::READ;
}

fetch_word * inst_word::get_fetch_ops() const{
    return m_fetch_wrd.get();
}

decode_word * inst_word::get_decode_ops() const{
    return m_decode_wrd.get();
}

addrr_word * inst_word::get_ar_ops() const{
    return m_addrr_wrd.get();
}

exec_word * inst_word::get_exec_ops() const{
    return m_exec_wrd.get();
}

bool inst_word::get_write() const{
    return m_ram_mode == ram_mode::WRITE;
}

void inst_word::set_word(msim_wrd * wrd) {
    inst_word* other = dynamic_cast<inst_word*>(wrd);
    if(other == nullptr) throw std::runtime_error("instruction word can not be set to a different class !");

    m_ram_mode = other->m_ram_mode;
    m_fetch_wrd->set_word(other->m_fetch_wrd.get());
    m_decode_wrd->set_word(other->m_decode_wrd.get());
    m_addrr_wrd->set_word(other->m_addrr_wrd.get());
    m_exec_wrd->set_word(other->m_exec_wrd.get());
}

void inst_word::set_or(msim_wrd * wrd) {
    inst_word* other = dynamic_cast<inst_word*>(wrd);
    if(other == nullptr) throw std::runtime_error("instruction word can not be set to a different class !");

    m_ram_mode = m_ram_mode == ram_mode::WAIT
               ? other->m_ram_mode
               : ram_mode::WAIT;

    m_fetch_wrd->set_or(other->m_fetch_wrd.get());
    m_decode_wrd->set_or(other->m_decode_wrd.get());
    m_addrr_wrd->set_or(other->m_addrr_wrd.get());
    m_exec_wrd->set_or(other->m_exec_wrd.get());
}


bool inst_word::ok() const {
    return m_ok &&
           (m_fetch_wrd ? m_fetch_wrd->ok() : true) &&
           (m_decode_wrd ? m_decode_wrd->ok() : true) &&
           (m_addrr_wrd ? m_addrr_wrd->ok() : true) &&
           (m_exec_wrd ? m_exec_wrd->ok() : true);
}

std::string inst_word::err_msg() {
    return msim_wrd::err_msg() +
           (m_fetch_wrd ? m_fetch_wrd->err_msg() : "") +
           (m_decode_wrd ? m_decode_wrd->err_msg() : "") +
           (m_addrr_wrd ? m_addrr_wrd->err_msg() : "") +
           (m_exec_wrd ? m_exec_wrd->err_msg() : "");
}
