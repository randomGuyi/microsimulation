#include "msim_rom.h"

#define RESIZE_SIZE 16

msim_rom::msim_rom(std::string const & id, std::string const & label)
    : msim_component{id, label}
    , m_index{-1}
    , m_size{0}
{}

void msim_rom::add_inst(inst_word && i_word, int line){
    if(line < 0) throw std::runtime_error("rom: line index out of bounds");

    while(static_cast<std::size_t>(line) >= m_instructions.size()){
        m_instructions.resize( m_instructions.size() + static_cast<std::size_t>(RESIZE_SIZE) );
    }
    if(! m_instructions[line]) ++m_size;

    m_instructions[line] = std::make_unique<inst_word>(std::move(i_word));
}

void msim_rom::next(){
    if(! has_next()){
        throw std::runtime_error("invalid call to rom::next()");
    }
    do{
        ++ m_index;
    }while(static_cast<std::size_t>(m_index) < m_instructions.size() &&
             m_instructions[m_index] == nullptr);
}

bool msim_rom::has_next() const {
    for(std::size_t i = m_index + 1; i < m_instructions.size(); ++i){
        if(m_instructions[i] != nullptr){
            return true;
        }
    }
    return false;
}

void msim_rom::back(){
    if(! has_prev()){
        throw std::runtime_error("invalid call to rom::back()");
    }
    do{
        --m_index;
    }while(m_index > 0 &&
             m_instructions[m_index] == nullptr);

}

bool msim_rom::has_prev() const{
    for(int i = m_index -1; i >= 0; --i){
        if(m_instructions[i] != nullptr){
            return true;
        }
    }
    return false;
}

bool msim_rom::is_valid_line(int line) const {
    return line >= 0 &&
           static_cast<std::size_t>(line) < m_instructions.size() &&
           m_instructions[line] != nullptr;
}

void msim_rom::reset_to_line(int line){
    if(is_valid_line(line)){
        m_index = line ;
    }else{
        throw std::runtime_error("line index out of bounds for rom instructions");
    }
}

inst_word const * msim_rom::get_current_instruction() const {
    return  (static_cast<std::size_t>(m_index) < m_instructions.size() && m_instructions[m_index]) ?
               m_instructions[m_index].get() :
               nullptr;
}

std::vector<std::unique_ptr<inst_word>> const & msim_rom::get_all_instructions() const{
    return m_instructions;
}

int msim_rom::get_current_line() const{
    return m_index;

}

bool msim_rom::is_at_first_instruction() const {
    return (m_size == 0 || m_size == 1) || (m_size > 0 && !has_prev());
}

bool msim_rom::is_at_last_instruction() const {
    return (m_size == 0 || m_size == 1) || (m_size > 0 && !has_next());
}

