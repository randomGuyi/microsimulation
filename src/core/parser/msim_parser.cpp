#include "msim_parser.h"

#include <qDebug>

msim_parser::msim_parser(msim_scanner * scanner_inst, msim_rom * rom_inst)
    :m_scanner{scanner_inst}
    , m_rom{rom_inst}
    , m_lookahead{}

{}
void msim_parser::set_rom_inst(msim_rom * inst){
    m_rom = inst;
}

void msim_parser::set_scanner_inst(msim_scanner * inst){
    m_scanner = inst;
    m_errors.clear();
    std::cout << " rename set_scanner to reset parser !!!" << std::endl;
}

bool msim_parser::has_error(){
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
    int line{0};
    while(m_lookahead.type != token_type::EOF_SY){
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

    // Pflicht: line number
    if(m_lookahead.type == token_type::NUMBER_SY){
        number(line);
        if(seg_set && !check_segment(seg, line)) {
            syntax_error("line does not match segment");
            return;
        }
    } else {
        syntax_error("line identifier [0-9]+");
        return;
    }

    // Pflicht: ;
    if(m_lookahead.type == token_type::SEMICOLON_SY){
        next_token();
    } else if (m_lookahead.type == token_type::EOF_SY){
        // TODO: new shit
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

            bool r{};
            ram_read(r);
            word.set_read(r);
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
        fetch_word fw{};
        fetch_sec(fw);
        word.set_fetch_ops(fw);

        if(m_lookahead.type == token_type::SEMICOLON_SY){
            next_token(); // ;
        }else{
            syntax_error("; after fetch instruction");
        }
    }

    /* [ DecodeSec ] */
    if(m_lookahead.type == token_type::R_SY){
        decode_word dw{};
        addrr_word aw{};
        decode_sec(dw, aw);
        word.set_decode_ops(dw);
        word.set_ar_ops(aw);

        if(m_lookahead.type == token_type::SEMICOLON_SY){
            next_token(); // ;
        }else{
            syntax_error("; after decode instruction");
        }
    }

    /* [ ExecSec ] */
    if(m_lookahead.type == token_type::B_SY){
        exec_word ew{};
        exec_sec(ew);
        word.set_exec_ops(ew);

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
            word.set_write(write);

            if(m_lookahead.type == token_type::SEMICOLON_SY){
                next_token(); // ;
            }else{
                syntax_error("; after ram write instruction");
            }
        }
    }
}

void msim_parser::ram_read( bool & r_opt){
    bool read {false};
    if(m_lookahead.type == token_type::RAM_SY){
        next_token();
        ram_read_op(read);
    }else{
        syntax_error("missing 'RAM:'");
        return;
    }
    r_opt = read;
};

void msim_parser::ram_read_op(bool & read){
        switch(m_lookahead.type){
        case token_type::READ_SY: {
                read = true;
                next_token();
                break;
            }
        case token_type::WAIT_SY:
            // case token_type::WARTEN_SY:
            {
                read = false;
                next_token();
                break;
            }
        default: {
                syntax_error("read instruction after 'RAM:'");
                return;
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

void msim_parser::fetch_sec(fetch_word & f_word){
    if(m_lookahead.type == token_type::H_SY){
        next_token();
    }else{
        syntax_error("fetch symbol [H: / F:]");
        return;
    }

    fetch_word fw{};
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
        fetch_cmd_list(fw);
        f_word = fw;
    }
}

void msim_parser::fetch_cmd_list(fetch_word & fetch_wrd){

//    fetch_word w{};
    fetch_word fetch_cd{};
    fetch_cmd(fetch_cd);
    fetch_wrd.set_or(&fetch_cd);
    if(! fetch_wrd.ok()){
        semantic_error(fetch_wrd.err_msg());
    }

    while(m_lookahead.type == token_type::COMMA_SY){
        next_token();
        fetch_word fetch_cd{};
        fetch_cmd(fetch_cd);
        fetch_wrd.set_or(&fetch_cd);
        if(! fetch_wrd.ok()){
            semantic_error(fetch_wrd.err_msg());
        }
    }
};

void msim_parser::fetch_cmd (fetch_word & fetch_c){
    fetch_word w{};
    switch(m_lookahead.type){
        case token_type::R0_TO_X_SY:
        case token_type::R1_TO_X_SY:
        case token_type::R2_TO_X_SY:
        case token_type::R3_TO_X_SY:{
            uint8_t x_nbl{0};
            x_sel(x_nbl);
            w.set_x_sel(x_nbl);
            break;
        }
        case token_type::R0_TO_Y_SY:
        case token_type::R1_TO_Y_SY:
        case token_type::R2_TO_Y_SY:
        case token_type::R3_TO_Y_SY:{
            uint8_t y_nbl{0};
            y_sel(y_nbl);
            w.set_y_sel(y_nbl);
            break;
        }
        case token_type::MDR_TO_COP_SY:
        case token_type::MDR_TO_Y_SY:{
            uint8_t mdr_nbl{0};
            mdr_opt(mdr_nbl);
            w.set_mdr_sel(mdr_nbl);
            break;
        }
        default: {
            syntax_error("XBus Selection, YBus Selection or MDR Selection");
            return;
        }
    };
        fetch_c = w;
};

void msim_parser::x_sel(uint8_t & x_nbl){
    uint8_t bt{0};
    switch(m_lookahead.type){
    case token_type::R0_TO_X_SY:{
        set_bit(bt, 0);
        next_token();
        break;
    }
    case token_type::R1_TO_X_SY:{
        set_bit(bt, 1);
        next_token();
        break;
    }
    case token_type::R2_TO_X_SY:{
        set_bit(bt, 2);
        next_token();
        break;
    }
    case token_type::R3_TO_X_SY:{
        set_bit(bt, 3);
        next_token();
        break;
    }
    default:
        syntax_error("XBus selection");
        return;
    }
    x_nbl =  bt;
};

void msim_parser::y_sel(uint8_t & y_nbl){
    uint8_t bt{0};
    switch(m_lookahead.type){
    case token_type::R0_TO_Y_SY:{
        set_bit(bt, 0);
        next_token();
        break;
    }
    case token_type::R1_TO_Y_SY:{
        set_bit(bt, 1);
        next_token();
        break;
    }
    case token_type::R2_TO_Y_SY:{
        set_bit(bt, 2);
        next_token();
        break;
    }
    case token_type::R3_TO_Y_SY:{
        set_bit(bt, 3);
        next_token();
        break;
    }
    default:
        syntax_error("YBus selection");
        return;
    }
    y_nbl =  bt;
};

void msim_parser::mdr_opt(uint8_t & mdr_nbl){
    uint8_t bt{0};
    switch (m_lookahead.type) {
    case token_type::MDR_TO_COP_SY:{
        next_token();
        set_bit(bt, 1);
        break;
    }
    case token_type::MDR_TO_Y_SY:{
        next_token();
        set_bit(bt, 0);
        break;
    }
    default:
        syntax_error("MDR Selection");
        return;
    }
    mdr_nbl= bt;
};

void msim_parser::decode_sec(decode_word & dec_word, addrr_word & ar_wrd ){
    if(m_lookahead.type == token_type::R_SY){
        next_token();
    }else{
        syntax_error("Decode Sequence");
        return;
    }
    decode_word dw{};
    addrr_word aw{};
    if(m_lookahead.type == token_type::Z_ASSIGN_SY ||
       m_lookahead.type == token_type::CAR_PLS_PLS_SY ||
        m_lookahead.type == token_type::ASSIGN_CAR_SY){

        decode_cmd_list(dw, aw);
    }
    dec_word = dw;
    ar_wrd = aw;
};

void msim_parser::decode_cmd_list( decode_word & dec_cmd, addrr_word & ar_cmd){
    decode_word dw{};
    addrr_word aw{};

    if(m_lookahead.type == token_type::Z_ASSIGN_SY ||
       m_lookahead.type == token_type::CAR_PLS_PLS_SY||
        m_lookahead.type == token_type::ASSIGN_CAR_SY){

        decode_cmd(dw, aw);
        dec_cmd.set_word(&dw);
        ar_cmd.set_or(&aw);
        if(! dec_cmd.ok()){
            semantic_error(dec_cmd.err_msg());
        }
        if(! ar_cmd.ok()){
            semantic_error(ar_cmd.err_msg());
        }
    }else{
        syntax_error("Decode Sequence");
        return;
    }

    while(m_lookahead.type == token_type::COMMA_SY){
        next_token();
        decode_cmd(dw, aw);
        dw.set_word(&dw);
        aw.set_or(&aw);
        if(! dw.ok()){
            semantic_error(dw.err_msg());
        }
        if(! aw.ok()){
            semantic_error(aw.err_msg());
        }
    }


};


void msim_parser::decode_cmd(decode_word & dec_word, addrr_word & ar_word){
    decode_word dw{};
    addrr_word aw{};

    if(m_lookahead.type == token_type::Z_ASSIGN_SY){
        decode_expr(dw);
        if(! dw.ok()){
            semantic_error(dw.err_msg());
        }
    }else if(m_lookahead.type == token_type::CAR_PLS_PLS_SY ||
               m_lookahead.type == token_type::ASSIGN_CAR_SY){  /* do dada */
        ar_sec(aw);
        if(! aw.ok()){
            semantic_error(aw.err_msg());
        }
    }else{
        syntax_error("Z assign 'Z:=' or CAR Selection ");
        return;
    }
    dec_word = dw;
    ar_word = aw;
};

void msim_parser::decode_expr (decode_word & dec_cmd){

    if(m_lookahead.type == token_type::Z_ASSIGN_SY){
        next_token();
    }else{
        syntax_error("Z assign 'Z:='");
        return;
    }
    decode_word dw{};
    switch(m_lookahead.type){
        case token_type::NUMBER_SY:{
            int n{0};
            number(n);
            dw.set_const_number(n);
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
                dw.set_operation(Z_X);
            }else{
                dw.set_operation(tail);
            }
            break;
        }
        case token_type::Y_SY:{
            next_token();
            dw.set_operation(Z_Y);
            break;
        }
        case token_type::INC_SY:{
            next_token();
            if(m_lookahead.type == token_type::X_SY){
                next_token();
                dw.set_operation(Z_INC_X);
            }else if(m_lookahead.type == token_type::Y_SY){
                next_token();
                dw.set_operation(Z_INC_Y);
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
                dw.set_operation(Z_DEC_X);
            }else if(m_lookahead.type == token_type::Y_SY){
                next_token();
                dw.set_operation(Z_DEC_Y);
            }else{
                syntax_error("'X' or 'Y'");
                return;
            }
            break;
        }
        case token_type::MIN_X_SY:{
            next_token();
            dw.set_operation(Z_MINUS_X);
            break;
        }
        case token_type::Z_SY:{
            next_token();
            dw.set_default();
            break;
        }
        default: {
            semantic_error("invalid decode expression");
            return;
        }
    }
    dec_cmd = dw;
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

void msim_parser::ar_sec(addrr_word & ar_cmd){
    addrr_word aw{};
    if(m_lookahead.type == token_type::ASSIGN_CAR_SY){
        next_token();
        ar_assign(ar_cmd);
    }else if(m_lookahead.type == token_type::CAR_PLS_PLS_SY){
        next_token();
        aw.set_default();
    }else{
        syntax_error("'CAR:=' or 'CAR++'");
        return;
    }

    ar_cmd = aw;

};

void msim_parser::ar_assign (addrr_word & ar_ass){
    int cn = {-1};
    uint8_t mask = {0};
    bool mask_set {false};
    if(m_lookahead.type == token_type::_4_CN_SY){
        next_token();
        if(m_lookahead.type == token_type::COMMA_SY){
            next_token();
            if(m_lookahead.type == token_type::CN_ASSIGN_SY){
                next_token();
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
        }
        if(cn > 0){
            ar_ass.set_cn(cn);
        }else{
            ar_ass.set_cn();
        }
    }else if(m_lookahead.type == token_type::_4COP_SY){
        next_token();
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
        if(mask_set){
            ar_ass.set_mask(mask);
        }else{
            ar_ass.set_mask();
        }

    }else{
        syntax_error("'4CN' '4COP'");
        return;
    }
};
void msim_parser::exec_sec( exec_word & ex_wrd){
    if(m_lookahead.type == token_type::B_SY){
        next_token();
    }else{
        syntax_error("'B'");
        return;
    }
    exec_word w{};
    if( m_lookahead.type == token_type::Z_R0_SY ||
        m_lookahead.type == token_type::Z_R1_SY ||
        m_lookahead.type == token_type::Z_R2_SY ||
        m_lookahead.type == token_type::Z_R3_SY ||
        m_lookahead.type == token_type::Z_MAR_SY||
        m_lookahead.type == token_type::Z_MDR_SY )
    {
        exec_list(w);
    }

    ex_wrd = w;
};

void msim_parser::exec_list (exec_word & exec_opt){
    exec_word ew{};

    if( m_lookahead.type == token_type::Z_R0_SY ||
        m_lookahead.type == token_type::Z_R1_SY ||
        m_lookahead.type == token_type::Z_R2_SY ||
        m_lookahead.type == token_type::Z_R3_SY ||
        m_lookahead.type == token_type::Z_MAR_SY||
        m_lookahead.type == token_type::Z_MDR_SY )
    {
        uint8_t exec_nibble{0};
        exec_op(exec_nibble);
        ew.set_nibble(exec_nibble);
        if(! ew.ok()){
            semantic_error(ew.err_msg());
            return;
        }
        while(m_lookahead.type == token_type::COMMA_SY){
            next_token();
            uint8_t exec_nibble{0};
            exec_op(exec_nibble);
            ew.set_or_nibble(exec_nibble);
            if(! ew.ok()){
                semantic_error(ew.err_msg());
                return;
            }
        }
    }else{
        syntax_error("Z->[any register]");
        return;
    }
    exec_opt = ew;
};

void msim_parser::exec_op (uint8_t & exec_nibble){
    uint8_t bt{0};
    switch(m_lookahead.type){
        case token_type::Z_R0_SY :{
            next_token();
            set_bit(bt, 5);
            break;
        }
        case token_type::Z_R1_SY :{
            next_token();
            set_bit(bt, 4);
            break;
        }
        case token_type::Z_R2_SY :{
            next_token();
            set_bit(bt, 3);
            break;
        }
        case token_type::Z_R3_SY :{
            next_token();
            set_bit(bt, 2);
            break;
        }
        case token_type::Z_MAR_SY:{
            next_token();
            set_bit(bt, 1);
            break;
        }
        case token_type::Z_MDR_SY: {
            next_token();
            set_bit(bt, 0);
            break;
        }
        default: {
            syntax_error("Z->[any register]");
            return;
        }
    }
    exec_nibble = bt;
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
//    qDebug() << "next token called! prev: " << m_lookahead.value;
    if(m_scanner == nullptr){
        throw std::runtime_error("scanner in parser not set");
    }
    m_lookahead = m_scanner->get_next_token();
 //   qDebug() << "                                     current: " << m_lookahead.value;
};
std::vector<parser_error> msim_parser::get_errors(){
    return m_errors;
}
token msim_parser::peek_next_token(){
    if(m_scanner == nullptr){
        throw std::runtime_error("scanner in parser not set");
    }
    return m_scanner->peek_next_token();
}

void msim_parser::expect(token_type expected){
};
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
        throw std::runtime_error("no rom instance in parser set");
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

