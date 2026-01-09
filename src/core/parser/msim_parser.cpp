/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#include "msim_parser.h"

#include <iostream>
#include <qDebug>

using namespace core::parser;

msim_parser::msim_parser(msim_scanner * scanner_inst, components::msim_rom * rom_inst)
    :m_scanner{scanner_inst}
    , m_rom{rom_inst}
    , m_lookahead{}
{}

void msim_parser::set_rom_inst(components::msim_rom * inst){
    m_rom = inst;
}

void msim_parser::set_scanner_inst(msim_scanner * inst){
    m_scanner = inst;
    m_errors.clear();
}

bool msim_parser::has_error() const {
    return ! m_errors.empty();
}

void msim_parser::parse(){
    next_token();
    program();
    if(m_lookahead.type != token_type::EOF_SY){
        syntax_error("Unexpected token after end of program");
    }
}

void msim_parser::program(){
    while(m_lookahead.type != token_type::EOF_SY){
        int line{0};
        inst_word w{};
        segment(line, w);
        emit_instruction(w, line);
    }
};

void msim_parser::segment(int & line, inst_word & iw){
    bool seg_set = false;
    int seg = 0;

    // optional "# num /"
    if(m_lookahead.type == token_type::HASH_SY){
        next_token();
        if(m_lookahead.type != token_type::NUMBER_SY){
            syntax_error("number after '#'");
            return;
        }
        number(seg);
        seg_set = true;

        if(m_lookahead.type == token_type::SLASH_SY){
            next_token();
        } else {
            syntax_error("'/' after segment identifier");
            return;
        }
    }

    // line number
    if(m_lookahead.type == token_type::NUMBER_SY){
        number(line);
        if(seg_set && !check_segment(seg, line)) {
            semantic_error("line does not match segment");
            return;
        }
    } else {
        syntax_error("line identifier [0-9]+");
        return;
    }

    //  ;
    if(m_lookahead.type == token_type::SEMICOLON_SY){
        next_token();
    } else if (m_lookahead.type == token_type::EOF_SY){
        return;
    }
    else {
        syntax_error("new line after segment");
        return;
    }

    // InstructionBlock
    instruction_block(iw);
}

void msim_parser::instruction_block(inst_word & word){
    bool read_set{false};

    /* [RAMRead] */
    if(m_lookahead.type == token_type::RAM_SY){
        token_type next = peek_next_token().type;
        if( next == token_type::READ_SY  ||
            next == token_type::WAIT_SY){

            bool r {ram_read()};
            word.set_ram_mode(r ? ram_mode::READ : ram_mode::WAIT);
            read_set = true;
        }
        if(m_lookahead.type == token_type::SEMICOLON_SY){
            next_token(); // ;
        }else{
            syntax_error("; after ram read instruction");
        }
    }
    /* [ FetchSec ] */
    if(m_lookahead.type == token_type::H_SY){
        fetch_sec(word);

        if(m_lookahead.type == token_type::SEMICOLON_SY){
            next_token(); // ;
        }else{
            syntax_error("; after fetch instruction");
        }
    }

    /* [ DecodeSec ] */
    if(m_lookahead.type == token_type::R_SY){
        decode_sec(word);

        if(m_lookahead.type == token_type::SEMICOLON_SY){
            next_token(); // ;
        }else{
            syntax_error("; after decode instruction");
        }
    }

    /* [ ExecSec ] */
    if(m_lookahead.type == token_type::B_SY){
        exec_sec(word);

        if(m_lookahead.type == token_type::SEMICOLON_SY){
            next_token(); // ;
        }else{
            syntax_error("; after execute instruction");
        }
    }

    /* [ RAMWrite ] */
    if(m_lookahead.type == token_type::RAM_SY){
        token_type next = peek_next_token().type;
        if( next == token_type::WRITE_SY||
            next == token_type::WAIT_SY ){

            if(read_set){
                semantic_error("RAM mode 'read' already set");
                return;
            }
            bool write{};
            ram_write(write);
            word.set_ram_mode(write ? ram_mode::WRITE : ram_mode::WAIT);

            if(m_lookahead.type == token_type::SEMICOLON_SY){
                next_token(); // ;
            }else{
                syntax_error("; after ram write instruction");
            }
        }
    }
}

bool msim_parser::ram_read(){
    bool read {false};
    if(m_lookahead.type == token_type::RAM_SY){
        next_token();
        ram_read_op(read);
    }else{
        syntax_error("missing 'RAM:'");
        return false;
    }
    return read;
};

void msim_parser::ram_read_op(bool & read){
        switch(m_lookahead.type){
        case token_type::READ_SY: {
                read = true;
                next_token();
                break;
            }
        case token_type::WAIT_SY:
            {
                read = false;
                next_token();
                break;
            }
        default: {
                syntax_error("read instruction after 'RAM:'");
            }
        }
};

void msim_parser::ram_write(bool & w_opt){
    bool write {false};
    if(m_lookahead.type == token_type::RAM_SY){
        next_token();
        ram_write_opt(write);
    }else{
        syntax_error("missing 'RAM:'");
        return;
    }
    w_opt = write;
};

void msim_parser::ram_write_opt(bool & write){
    write = m_lookahead.type == token_type::WRITE_SY;
    // consume write
    next_token();
};

void msim_parser::fetch_sec(inst_word &f_word){
    if(m_lookahead.type == token_type::H_SY){
        next_token();
    }else{
        syntax_error("fetch symbol [H: / F:]");
        return;
    }

    if( m_lookahead.type == token_type::R0_TO_X_SY ||
        m_lookahead.type == token_type::R1_TO_X_SY ||
        m_lookahead.type == token_type::R2_TO_X_SY ||
        m_lookahead.type == token_type::R3_TO_X_SY ||

        m_lookahead.type == token_type::R0_TO_Y_SY ||
        m_lookahead.type == token_type::R1_TO_Y_SY ||
        m_lookahead.type == token_type::R2_TO_Y_SY ||
        m_lookahead.type == token_type::R3_TO_Y_SY ||

        m_lookahead.type == token_type::MDR_TO_COP_SY||
        m_lookahead.type == token_type::MDR_TO_Y_SY
        )
    {
        fetch_cmd_list(f_word);
    }
}

void msim_parser::fetch_cmd_list(inst_word &fetch_wrd){

    fetch_cmd(fetch_wrd);
    if(! fetch_wrd.ok()){
        semantic_error(fetch_wrd.err_msg());
    }

    while(m_lookahead.type == token_type::COMMA_SY){
        next_token();
        fetch_cmd(fetch_wrd);
        if(! fetch_wrd.ok()){
            semantic_error(fetch_wrd.err_msg());
        }
    }
};

void msim_parser::fetch_cmd (inst_word &fetch_c){
    switch(m_lookahead.type){
        case token_type::R0_TO_X_SY:
        case token_type::R1_TO_X_SY:
        case token_type::R2_TO_X_SY:
        case token_type::R3_TO_X_SY:{
            x_sel(fetch_c);
            break;
        }
        case token_type::R0_TO_Y_SY:
        case token_type::R1_TO_Y_SY:
        case token_type::R2_TO_Y_SY:
        case token_type::R3_TO_Y_SY:{
            y_sel(fetch_c);
            break;
        }
        case token_type::MDR_TO_COP_SY:
        case token_type::MDR_TO_Y_SY:{
            mdr_opt(fetch_c);
            break;
        }
        default: {
            syntax_error("XBus Selection, YBus Selection or MDR Selection");
            return;
        }
    };
};

void msim_parser::x_sel(inst_word &wrd){
    switch(m_lookahead.type){
    case token_type::R0_TO_X_SY:{
        wrd.set_x_selection(REGISTER_0);
        break;
    }
    case token_type::R1_TO_X_SY:{
        wrd.set_x_selection(REGISTER_1);
        break;
    }
    case token_type::R2_TO_X_SY:{
        wrd.set_x_selection(REGISTER_2);
        break;
    }
    case token_type::R3_TO_X_SY:{
        wrd.set_x_selection(REGISTER_3);
        break;
    }
    default:
        syntax_error("XBus selection");
        return;
    }
    next_token();
};

void msim_parser::y_sel(inst_word &y_nbl){
    switch(m_lookahead.type){
    case token_type::R0_TO_Y_SY:{
        y_nbl.set_y_selection(REGISTER_0);
        break;
    }
    case token_type::R1_TO_Y_SY:{
        y_nbl.set_y_selection(REGISTER_1);
        break;
    }
    case token_type::R2_TO_Y_SY:{
        y_nbl.set_y_selection(REGISTER_2);
        break;
    }
    case token_type::R3_TO_Y_SY:{
        y_nbl.set_y_selection(REGISTER_3);
        break;
    }
    default:
        syntax_error("YBus selection");
        return;
    }
    next_token();
};

void msim_parser::mdr_opt(inst_word &mdr_nbl){
    switch (m_lookahead.type) {
    case token_type::MDR_TO_COP_SY:{
        next_token();
        mdr_nbl.set_mdr_cop(true);
        break;
    }
    case token_type::MDR_TO_Y_SY:{
        next_token();
        mdr_nbl.set_mdr_y(true);
        break;
    }
    default:
        syntax_error("MDR Selection");
    }
};

void msim_parser::decode_sec(inst_word &wrd){
    if(m_lookahead.type == token_type::R_SY){
        next_token();
    }else{
        syntax_error("Decode Sequence");
        return;
    }
    if(m_lookahead.type == token_type::Z_ASSIGN_SY ||
       m_lookahead.type == token_type::CAR_PLS_PLS_SY ||
        m_lookahead.type == token_type::ASSIGN_CAR_SY){

        decode_cmd_list(wrd);
    }
};

void msim_parser::decode_cmd_list(inst_word &wrd){

    if(m_lookahead.type == token_type::Z_ASSIGN_SY ||
       m_lookahead.type == token_type::CAR_PLS_PLS_SY||
        m_lookahead.type == token_type::ASSIGN_CAR_SY){

        decode_cmd(wrd);
        if(! wrd.ok()){
            semantic_error(wrd.err_msg());
        }
    }else{
        syntax_error("Decode Sequence");
        return;
    }

    while(m_lookahead.type == token_type::COMMA_SY){
        next_token();
        decode_cmd(wrd);
        if(! wrd.ok()){
            semantic_error(wrd.err_msg());
        }
    }
};


void msim_parser::decode_cmd(inst_word &wrd){

    if(m_lookahead.type == token_type::Z_ASSIGN_SY){
        decode_expr(wrd);
        if(! wrd.ok()){
            semantic_error(wrd.err_msg());
        }
    }else if(m_lookahead.type == token_type::CAR_PLS_PLS_SY ||
               m_lookahead.type == token_type::ASSIGN_CAR_SY){  /* do dada */
        ar_sec(wrd);
        if(! wrd.ok()){
            semantic_error(wrd.err_msg());
        }
    }else{
        syntax_error("Z assign 'Z:=' or CAR Selection ");
        return;
    }
};

void msim_parser::decode_expr (inst_word &wrd){

    if(m_lookahead.type == token_type::Z_ASSIGN_SY){
        next_token();
    }else{
        syntax_error("Z assign 'Z:='");
        return;
    }
    switch(m_lookahead.type){
        case token_type::NUMBER_SY:{
            int n{0};
            number(n);
            wrd.set_constant_nbr(n);
            wrd.set_operation(Z_CONST);
            break;
        }
        case token_type::X_SY:{
            uint8_t tail{0};
            next_token();
            if( m_lookahead.type == token_type::PLS_Y_SY ||
                m_lookahead.type == token_type::MIN_Y_SY ||
                m_lookahead.type == token_type::TIMES_Y_SY||
                m_lookahead.type == token_type::DIV_Y_SY ||
                m_lookahead.type == token_type::SHL_Y_SY )
            {
                x_tail(tail);
            }

            if(tail == 0){
                /* pass through */
                wrd.set_operation(Z_X);
            }else{
                /* actual operation */
                wrd.set_operation(tail);
            }
            break;
        }
        case token_type::Y_SY:{
            next_token();
            wrd.set_operation(Z_Y);
            break;
        }
        case token_type::INC_SY:{
            next_token();
            if(m_lookahead.type == token_type::X_SY){
                next_token();
                wrd.set_operation(Z_INC_X);
            }else if(m_lookahead.type == token_type::Y_SY){
                next_token();
                wrd.set_operation(Z_INC_Y);
            }else{
                syntax_error("'X' or 'Y'");
                return;
            }
            break;
        }
        case token_type::DEC_SY:{
            next_token();
            if(m_lookahead.type == token_type::X_SY){
                next_token();
                wrd.set_operation(Z_DEC_X);
            }else if(m_lookahead.type == token_type::Y_SY){
                next_token();
                wrd.set_operation(Z_DEC_Y);
            }else{
                syntax_error("'X' or 'Y'");
                return;
            }
            break;
        }
        case token_type::MIN_X_SY:{
            next_token();
            wrd.set_operation(Z_MINUS_X);
            break;
        }
        case token_type::Z_SY:{
            next_token();
            wrd.set_default_operation();
            break;
        }
        default: {
            semantic_error("invalid decode expression");
            return;
        }
    }
};

void msim_parser::x_tail(uint8_t & t){
    switch(m_lookahead.type){
        case token_type::PLS_Y_SY:{
            next_token();
            t = Z_X_PLS_Y;
            break;
        }
        case token_type::MIN_Y_SY:{
            next_token();
            t = Z_X_MINUS_Y;
            break;
        }
        case token_type::TIMES_Y_SY:{
            next_token();
            t = Z_X_TIMES_Y;
            break;
        }
        case token_type::DIV_Y_SY:{
            next_token();
            t = Z_X_DIV_Y;
            break;
        }
        case token_type::SHL_Y_SY:{
            next_token();
            t = Z_X_SHL_Y;
            break;
        }
        default: {
            syntax_error("'+Y', '-Y', '*Y', '/Y', '<<Y'");
            return;
    }

    }
};

void msim_parser::ar_sec(inst_word &wrd){
    if(m_lookahead.type == token_type::ASSIGN_CAR_SY){
        next_token();
        ar_assign(wrd);
    }else if(m_lookahead.type == token_type::CAR_PLS_PLS_SY){
        next_token();
        wrd.set_default_ar_opt();
    }else{
        syntax_error("'CAR:=' or 'CAR++'");
        return;
    }
};

void msim_parser::ar_assign (inst_word & wrd){
    int cn  {-1};
    uint8_t mask  {0};
    bool mask_set {false};
    if(m_lookahead.type == token_type::_4_CN_SY){
        next_token(); // consume 4CN
        if(m_lookahead.type == token_type::COMMA_SY){
            next_token(); // consume ,
            if(m_lookahead.type == token_type::CN_ASSIGN_SY){
                next_token(); // consume CN=
            }else{
                syntax_error("'CN='");
            }
            if(m_lookahead.type == token_type::NUMBER_SY){
                int n{0};
                number(n);
                cn = n;
            }else{
                syntax_error("number '[0-n]'");
                return;
            }
        }else{
            syntax_error("expected ',' found " + m_lookahead.value);
            return;
        }
        wrd.set_cn(cn);
        wrd.set_ar_mode(ar_mode::_4CN);

    }else if(m_lookahead.type == token_type::_4COP_SY){
        next_token(); // consume 4COP
        if(m_lookahead.type == token_type::IF_SY){
            next_token();
            if(m_lookahead.type == token_type::COND_NE0_SY){
                next_token();
            }else{
                syntax_error("'cond<0>'");
                return;
            }

            if(m_lookahead.type == token_type::MASKE_SY){
                next_token();
            }else{
                syntax_error("'MASKE'");
                return;
            }

            if(m_lookahead.type == token_type::EQ_SY){
                next_token();
            }else{
                syntax_error("'-'");
                return;
            }

            if(m_lookahead.type == token_type::NUMBER_SY){
                uint8_t n_mask{0};
                binary_number(n_mask);
                mask = n_mask;
                mask_set = true;
            }else{
                syntax_error("number");
            }
        }
        if (mask_set) {
            wrd.set_mask(mask);
            wrd.set_ar_mode(ar_mode::_4COP_CND);
        }else {
            wrd.set_ar_mode(ar_mode::_4COP);
        }

    }else{
        syntax_error("'4CN' '4COP'");
    }
};

void msim_parser::exec_sec(inst_word &wrd){
    if(m_lookahead.type == token_type::B_SY){
        next_token(); // consume B
    }else{
        syntax_error("'B'");
        return;
    }
    if( m_lookahead.type == token_type::Z_R0_SY ||
        m_lookahead.type == token_type::Z_R1_SY ||
        m_lookahead.type == token_type::Z_R2_SY ||
        m_lookahead.type == token_type::Z_R3_SY ||
        m_lookahead.type == token_type::Z_MAR_SY||
        m_lookahead.type == token_type::Z_MDR_SY )
    {
        exec_list(wrd);
    }
};

void msim_parser::exec_list (inst_word & word){

    if( m_lookahead.type == token_type::Z_R0_SY ||
        m_lookahead.type == token_type::Z_R1_SY ||
        m_lookahead.type == token_type::Z_R2_SY ||
        m_lookahead.type == token_type::Z_R3_SY ||
        m_lookahead.type == token_type::Z_MAR_SY||
        m_lookahead.type == token_type::Z_MDR_SY )
    {
        exec_op(word);
        if(! word.ok()){
            semantic_error(word.err_msg());
            return;
        }
        while(m_lookahead.type == token_type::COMMA_SY){
            next_token();
            exec_op(word);
            if(! word.ok()){
                semantic_error(word.err_msg());
                return;
            }
        }
    }else{
        syntax_error("Z->[any register]");
        return;
    }
};

void msim_parser::exec_op (inst_word &word){
    switch(m_lookahead.type){
        case token_type::Z_R0_SY :{
            next_token();
            word.set_z_selection(REGISTER_0);
            break;
        }
        case token_type::Z_R1_SY :{
            next_token();
            word.set_z_selection(REGISTER_1);
            break;
        }
        case token_type::Z_R2_SY :{
            next_token();
            word.set_z_selection(REGISTER_2);
            break;
        }
        case token_type::Z_R3_SY :{
            next_token();
            word.set_z_selection(REGISTER_3);
            break;
        }
        case token_type::Z_MAR_SY:{
            next_token();
            word.set_z_mar(true);
            break;
        }
        case token_type::Z_MDR_SY: {
            next_token();
            word.set_z_mdr(true);
            break;
        }
        default: {
            syntax_error("Z->[any register]");
        }
    }
};

void msim_parser::number(int & nbr){
    if(m_lookahead.type == token_type::NUMBER_SY){
        nbr = std::stoi(m_lookahead.value);
        next_token();
    }else{
        syntax_error("number --> [0-9]+");
    }
};
void msim_parser::next_token(){
    if(m_scanner == nullptr){
        throw std::runtime_error("scanner in parser not set");
    }
    m_lookahead = m_scanner->get_next_token();
};

std::vector<parser_error> msim_parser::get_errors(){
    return m_errors;
}
token msim_parser::peek_next_token() const {
    if(m_scanner == nullptr){
        throw std::runtime_error("scanner in parser not set");
    }
    return m_scanner->peek_next_token();
}

void msim_parser::syntax_error(const std::string& expected){
    if(m_scanner == nullptr){
        throw std::runtime_error("scanner in parser not set");
    }
    int line = m_scanner->get_curr_line();
    std::string msg{ "[Syntax Error] Expected: " + expected + ", found: " + m_lookahead.value};
    m_errors.push_back({line, msg});
    panic_mode_recovery();
};

void msim_parser::semantic_error(const std::string& msg){
    if(m_scanner == nullptr){
        throw std::runtime_error("scanner in parser not set");
    }
    int line = m_scanner->get_curr_line();
    std::string msg_{"[Semantic Error] " + msg };
    m_errors.push_back({line, msg_ });
    panic_mode_recovery();
    /* or may throw an error */
}

void msim_parser::panic_mode_recovery(){
    while(m_lookahead.type != token_type::EOF_SY &&
        m_lookahead.type != token_type::SEMICOLON_SY&&
           m_lookahead.type != token_type::HASH_SY){
        next_token();
    }
    if(m_lookahead.type == token_type::SEMICOLON_SY){
        next_token();
    }
};

void msim_parser::emit_instruction(inst_word & w, int line){
    if(m_rom == nullptr){
        throw std::runtime_error("no rom placed");
    }
    m_rom->add_inst(std::move(w), line);
};

bool msim_parser::check_segment(int seg, int line){
    return ((line / 4) == seg);
};

void msim_parser::set_bit(uint8_t &byte, int pos){
    byte |= (1 << pos);
};

void msim_parser::binary_number(uint8_t & digit){
    digit = 0;
    if(m_lookahead.type == token_type::NUMBER_SY){
        if(m_lookahead.value.size() != 4){
            semantic_error("invalid binary number (exactly 4 bits)");
            return;
        }
        for(int i = 0; i < 4; ++i){
            /*  mask = Z + - Of */
            char c = m_lookahead.value[i];
            if(c == '1'){
                digit |= (1u << (3 -i));
            }else if(c != '0'){
                semantic_error("invalid binary digit");
                return;
            }
        }
        next_token();
    }
}

