/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MSIM_INSTRUCTION_LINE_H
#define MSIM_INSTRUCTION_LINE_H

#include "core/components/msim_enable_bit.h"
#include "core/microcode/inst_word.h"
#include <map>
#include <string>

namespace core::components {
    class msim_instruction_line
    {

    public:
        static msim_instruction_line & get_instance(){
            static msim_instruction_line instance{};
            return instance;
        }

        msim_enable_bit * create_bit(std::string const & id, std::string const & label);
        msim_enable_bit * get_bit_for_id(std::string const & id);
        void load_next_instruction(inst_word const & word);
        void execute_current_instruction();


    private:
        msim_instruction_line();
        msim_instruction_line( msim_instruction_line const & other) = delete;
        msim_instruction_line & operator=(msim_instruction_line const & other) = delete;
        ~msim_instruction_line() = default;

        std::map<std::string, msim_enable_bit *> m_bits;
    };

}
#endif // MSIM_INSTRUCTION_LINE_H
