#ifndef MSIM_PARSER_H
#define MSIM_PARSER_H

#include "../microcode/inst_word.h"
#include "msim_scanner.h"
#include "../../core/components/msim_rom.h"

namespace core::parser {

struct parser_error{
    int line;
    std::string message;
};

class msim_parser
{
public:
    msim_parser(msim_scanner * scanner_inst, components::msim_rom * rom_inst);
    void parse();
    bool has_error() const;
    std::vector<parser_error> get_errors();
    void set_rom_inst(components::msim_rom * inst);
    void set_scanner_inst(msim_scanner * inst);
    static bool check_segment(int seg, int line);

private:

    void program();
    void segment(int & line, inst_word & iw);
    void instruction_block(inst_word & word);

    bool ram_read();
    void ram_read_op(bool & read);

    void ram_write(bool & w_opt);
    void ram_write_opt(bool & write);

    void fetch_sec(inst_word & f_word);
    void fetch_cmd_list(inst_word &fetch_wrd);
    void fetch_cmd (inst_word &fetch_c);

    void x_sel(inst_word &wrd);
    void y_sel(inst_word &y_nbl);
    void mdr_opt(inst_word &mdr_nbl);

    void decode_sec(inst_word &wrd);
    void decode_cmd_list(inst_word &wrd);
    void decode_cmd(inst_word &wrd);

    void decode_expr (inst_word &wrd);

    void x_tail(uint8_t & t);
    void ar_sec(inst_word &wrd);
    void ar_assign (inst_word &ar_ass);

    void exec_sec(inst_word &wrd);
    void exec_list (inst_word &exec_opt);
    void exec_op (inst_word &word);

    void number(int & nbr);
    void binary_number(uint8_t & digit);

    void next_token();
    token peek_next_token();

    void syntax_error(const std::string& expected);
    void semantic_error(const std::string& msg);
    void panic_mode_recovery();

    void emit_instruction(inst_word & w, int line);


    static void set_bit(uint8_t &byte, int pos);

    msim_scanner * m_scanner;
    components::msim_rom * m_rom;
    token m_lookahead;

    std::vector<parser_error> m_errors;

};

}
#endif // MSIM_PARSER_H
