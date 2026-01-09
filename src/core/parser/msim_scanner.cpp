#include "msim_scanner.h"
#include <algorithm>
#include <iterator>
#include <iostream>


/* debug mode */
// #define DEBUG_SCANNER

#ifdef DEBUG_SCANNER
    #include <iostream>
#endif
using namespace core::parser;
msim_scanner::msim_scanner(const std::string & source, bool ignore_comments)
    : m_pos{0}			   /* position in current stream */
    , m_source{}
    , m_normalized_line{1} /*  actual line in editor */
    , m_normalized_col{1}  /*  actual column in editor */
    , m_normalize_offset{0}/*  actual char position in editor */
    , m_ignore_comments{ignore_comments}
{
    std::transform(
        source.begin(),
        source.end(),
        std::back_inserter(m_source),
        [](unsigned char c){
            return static_cast<char>(std::toupper(c));
    });

}

int msim_scanner::get_curr_line() const {
    return m_normalized_line;
}

std::string msim_scanner::normalize_new_lines(const std::string & src){
    std::string result{};
    result.reserve(src.size());

    for(size_t i = 0; i < src.size(); ++i){
        char c = src[i];
        m_normalized_col ++;
        /* normalize new line */
        if(c == '\r'){
            if((i + 1) < src.size() && src[i + 1] == '\n'){
                ++ i;
                ++ m_normalize_offset;
            }
            result.push_back('\n');

            ++m_normalized_line;
            m_normalized_col = 0;
        }else{
            result.push_back(c);
        }
    }
    return result;
}

token msim_scanner::get_next_token(){
    skip_blanks();
    if(m_ignore_comments){
        skip_comment();
        skip_blanks(); //  for blanks after comment
    }
    if(is_at_end()){
        return make_token(token_type::EOF_SY, "", m_pos, m_normalized_line, m_normalized_col);
    }
    return scan_token();
}

token msim_scanner::peek_next_token(){
    size_t old_pos = m_pos;
    int old_line = m_normalized_line;
    int old_col = m_normalized_col;
    size_t old_normalize = m_normalize_offset;
    std::string old_last_error = m_last_error;

    token tkn = get_next_token();

    m_pos = old_pos;
    m_normalized_line = old_line;
    m_normalized_col = old_col;
    m_normalize_offset = old_normalize;
    m_last_error = old_last_error;

    return tkn;
}

bool msim_scanner::has_more_tokens() const{
    return m_pos < m_source.size();
}

void msim_scanner::reset(std::string const & text){
    m_pos = 0;
    m_normalized_col = 1;
    m_normalized_line = 1;
    m_normalize_offset = 0;
    m_source.clear();
    std::transform(
        text.begin(),
        text.end(),
        std::back_inserter(m_source),
        [](unsigned char c){
            return static_cast<char>(std::toupper(c));
    });
}

size_t msim_scanner::get_position() const{
    return m_pos + m_normalize_offset;
}

void msim_scanner::set_position(size_t pos){
    m_pos = pos;
}

std::vector<token> msim_scanner::get_tokens(){
    std::vector<token> tkns{};
    while (has_more_tokens()) {
        token tkn = get_next_token();
        tkns.push_back(tkn);
        if (tkn.type == token_type::EOF_SY) break;
    }
    if(tkns.empty() || tkns.back().type != token_type::EOF_SY){
        tkns.push_back(make_token(token_type::EOF_SY, std::string{EOS}, m_pos, m_normalized_line, m_normalized_col));
    }

    return tkns;
}

std::vector<token> msim_scanner::tokenize_line(size_t line_index){
    std::vector<token> result{};
    size_t start{0};
    size_t curr_line{0};

    while(start < m_source.size()){
        size_t end = m_source.find('\n', start);
        if(end == std::string::npos) end = m_source.size();

        if(curr_line == line_index){
            /* line found */
            msim_scanner line_scanner {m_source.substr(start, end - start), m_ignore_comments};
            return line_scanner.get_tokens();
        }

        start = end + 1;
        ++ curr_line;
    }
    return result;
}

token msim_scanner::get_token_at_position(int line, int col){
    size_t old_pos = m_pos;
    int old_line = m_normalized_line;
    int old_col = m_normalized_col;
    int old_norm_off = m_normalize_offset;

    m_pos = 0;
    m_normalized_line = 1;
    m_normalized_col = 1;
    m_normalize_offset = 0;

    token t{token_type::UNKNOWN, "", m_normalized_line, m_normalized_col, m_pos + m_normalize_offset};

    while (has_more_tokens()) {
        t = get_next_token();
        if (t.line == line && col >= t.column && col <= t.column + (int)t.value.size()) {
            break;
        }
    }

    m_pos = old_pos;
    m_normalized_line = old_line;
    m_normalized_col= old_col;
    m_normalize_offset = old_norm_off;

    return t;
}

std::string msim_scanner::get_last_error() const{
    return m_last_error;
}

void msim_scanner::skip_comment(){
    char c = peek_char();
    if(m_ignore_comments && c == '/' && peek_next_char() == '*'){
        advance();
        advance();
        c = peek_char();
        while(! is_at_end() && c != '*' && peek_next_char() != '/' ){
            advance();
            c = peek_char();
        }
        if(! is_at_end()){
            advance(); // *
            advance(); // /
        }

    }
}


void msim_scanner::skip_blanks(){
    while(! is_at_end()){
        char c = peek_char();
        if(c == '\r'){
            advance();
            continue;
        }
        if(c == ' '   ||
           c == '\t'  ||
           c == '\x01'||
           c == '\n'  ||
           c == '\r'  ){
            advance();
            continue;
        }
        break;
        }
}

token msim_scanner::make_token(token_type tkt, std::string const & val , size_t start_pos, int start_line, int start_col){
    if(val == "\n"){
        m_normalized_line ++;
        m_normalized_col = 1;
    }
    token t ;
    t.type = tkt;
    t.value = val;
    t.line = start_line;
    t.column = start_col;
    t.abs_pos = start_pos + m_normalize_offset;
    return t;

}

bool msim_scanner::match(const std::string & kw){
    if(m_source.compare(m_pos, kw.size(), kw) == 0){
        m_pos += kw.size();
        m_normalized_col += kw.size();
        return true;
    }
    return false;
}

char msim_scanner::peek_char() const{
    if(m_pos < m_source.size()){
        return m_source[m_pos];
    }
    return EOS;
}

char msim_scanner::peek_next_char() const {
    if(m_pos + 1 < m_source.size()){
        return m_source[m_pos + 1];
    }
    return EOS;
}

char msim_scanner::advance(){
    if(! is_at_end()){
        char c = m_source[m_pos++];
        if(c == '\n'){
            ++m_normalized_line;
            m_normalized_col = 1;
        } else{
            ++m_normalized_col;
        }
        return c;
    }
    return EOS;
}

bool msim_scanner::is_at_end() const {
    return m_pos >= m_source.size();
}

token msim_scanner::number_token(){
    size_t start = m_pos;
    int start_col =m_normalized_col;
    while(std::isdigit(peek_char())) advance();
    std::string val = m_source.substr(start, m_pos - start);
    return make_token(token_type::NUMBER_SY, val, start, m_normalized_line, start_col);
}

token msim_scanner::scan_token(){
    size_t start = m_pos;
    int start_line = m_normalized_line;
    int start_col = m_normalized_col;

    if(match( "SCHREIBEN")) return make_token(token_type::WRITE_SY, "schreiben", start, start_line, start_col); 		// "schreiben"
    if(match( "[MAR]->MDR")) return make_token(token_type::READ_SY, "[MAR]->MDR", start, start_line, start_col); 		// "[MAR]->MDR"
    if(match( "MDR->[MAR]")) return make_token(token_type::WRITE_SY, "MDR->[MAR]", start, start_line, start_col); 		// "MDR->[MAR]"
    if(match( "MDR->COP")) return make_token(token_type::MDR_TO_COP_SY, "MDR->COP", start, start_line, start_col); 		// "MDR->COP"
    if(match( "COND<>0")) return make_token(token_type::COND_NE0_SY, "cond<>0", start, start_line, start_col); 		// "cond<>0"
    if(match( "MDR->Y")) return make_token(token_type::MDR_TO_Y_SY, "MDR->Y", start, start_line, start_col); 		// "MDR->Y"
    if(match("WARTEN")) return make_token(token_type::WAIT_SY, "warten", start, start_line, start_col);
    if(match( "Z->MAR")) return make_token(token_type::Z_MAR_SY, "Z->MAR", start, start_line, start_col); 			// "Z->MAR"
    if(match( "Z->MDR")) return make_token(token_type::Z_MDR_SY, "Z->MDR", start, start_line, start_col); 		    // "Z->MDR"
    if(match( "LESEN")) return make_token(token_type::READ_SY, "lesen", start, start_line, start_col); 			// "lesen"
    if(match( "R0->X")) return make_token(token_type::R0_TO_X_SY, "R0->X", start, start_line, start_col); 		// "R0->X"
    if(match( "R1->X")) return make_token(token_type::R1_TO_X_SY, "R1->X", start, start_line, start_col); 		// "R1->X"
    if(match( "R2->X")) return make_token(token_type::R2_TO_X_SY, "R2->X", start, start_line, start_col); 		// "R2->X"
    if(match( "R3->X")) return make_token(token_type::R3_TO_X_SY, "R3->X", start, start_line, start_col); 		// "R3->X"
    if(match( "R0->Y")) return make_token(token_type::R0_TO_Y_SY, "R0->Y", start, start_line, start_col); 		// "R0->Y"
    if(match( "R1->Y")) return make_token(token_type::R1_TO_Y_SY, "R1->Y", start, start_line, start_col); 		// "R1->Y"
    if(match( "R2->Y")) return make_token(token_type::R2_TO_Y_SY, "R2->Y", start, start_line, start_col); 		// "R2->Y"
    if(match( "R3->Y")) return make_token(token_type::R3_TO_Y_SY, "R3->Y", start, start_line, start_col); 		// "R3->Y"
    if(match( "WRITE")) return make_token(token_type::WRITE_SY, "write", start, start_line, start_col);   		// "write"
    if(match( "CAR:=")) return make_token(token_type::ASSIGN_CAR_SY, "CAR:=", start, start_line, start_col); 		// "CAR:="
    if(match( "CAR++")) return make_token(token_type::CAR_PLS_PLS_SY, "CAR++", start, start_line, start_col); 	// "CAR++"
    if(match( "MASKE")) return make_token(token_type::MASKE_SY, "MASKE", start, start_line, start_col); 			// "MASKE"
    if(match( "Z->R0")) return make_token(token_type::Z_R0_SY, "Z->R0", start, start_line, start_col); 			// "Z->R0"
    if(match( "Z->R1")) return make_token(token_type::Z_R1_SY, "Z->R1", start, start_line, start_col); 			// "Z->R1"
    if(match( "Z->R2")) return make_token(token_type::Z_R2_SY, "Z->R2", start, start_line, start_col);  			// "Z->R2"
    if(match( "Z->R3")) return make_token(token_type::Z_R3_SY, "Z->R3", start, start_line, start_col); 			// "Z->R3"
    if(match( "READ")) return make_token(token_type::READ_SY, "read", start, start_line, start_col);  			// "read"
    if(match( "WAIT")) return make_token(token_type::WAIT_SY, "wait", start, start_line, start_col);     		// "wait"
    if(match( "4*COP")) return make_token(token_type::_4COP_SY, "4*COP", start, start_line, start_col); 		// "4COP"
    if(match( "4COP")) return make_token(token_type::_4COP_SY, "4COP", start, start_line, start_col); 		// "4COP"
    if(match( "RAM:")) return make_token(token_type::RAM_SY, "RAM:", start, start_line, start_col); 			// "RAM:"
    if(match( "INC")) return make_token(token_type::INC_SY, "INC", start, start_line, start_col); 			// "INC"
    if(match( "DEC")) return make_token(token_type::DEC_SY, "DEC", start, start_line, start_col); 			// "DEC"
    if(match( "<<Y")) return make_token(token_type::SHL_Y_SY, "<<Y", start, start_line, start_col); 			// "<<Y"
    if(match( "AR:")) return make_token(token_type::AR_SY, "AR:", start, start_line, start_col); 				// "AR:"
    if(match( "4*CN") ) return make_token(token_type::_4_CN_SY, "4*CN", start, start_line, start_col); 			// "4CN"
    if(match( "4CN") ) return make_token(token_type::_4_CN_SY, "4CN", start, start_line, start_col); 			// "4CN"
    if(match( "CN=")) return make_token(token_type::CN_ASSIGN_SY, "CN=", start, start_line, start_col);   	// "CN="
    if(match( "Z:=")) return make_token(token_type::Z_ASSIGN_SY, "Z:=", start, start_line, start_col); 		// "Z:="
    if(match( "-X")) return make_token(token_type::MIN_X_SY, "-X", start, start_line, start_col); 			// "-X"
    if(match( "+Y")) return make_token(token_type::PLS_Y_SY, "+Y", start, start_line, start_col); 			// "+Y"
    if(match( "-Y")) return make_token(token_type::MIN_Y_SY, "-Y", start, start_line, start_col); 			// "-Y"
    if(match( "*Y")) return make_token(token_type::TIMES_Y_SY, "*Y", start, start_line, start_col); 		    // "*Y"
    if(match( "/Y")) return make_token(token_type::DIV_Y_SY, "/Y", start, start_line, start_col); 			// "/Y"
    if(match( "IF")) return make_token(token_type::IF_SY, "IF", start, start_line, start_col);    			// "IF"
    if(match( "H:")) return make_token(token_type::H_SY, "H:", start, start_line, start_col); 				// "H:"
    if(match( "B:")) return make_token(token_type::B_SY, "B:", start, start_line, start_col); 				// "B:"
    // comment handling: use find for robustness
    if(! m_ignore_comments){
        if (m_source.compare(m_pos, 2, "/*") == 0) {
            size_t comment_start = m_pos;
            int c_line = m_normalized_line;
            int c_col = m_normalized_col;
            size_t endpos = m_source.find("*/", m_pos + 2);
            if (endpos == std::string::npos) {
                // unterminated comment: consume to end
                std::string val = m_source.substr(m_pos);
                m_pos = m_source.size();
                return make_token(token_type::COMMENT_SY, val, comment_start, c_line, c_col);
            } else {
                // include "*/"
                std::string val = m_source.substr(m_pos, endpos - m_pos + 2);
                // advance position correctly and update line/col via advance()
                size_t i = m_pos;
                while (i <= endpos + 1) { // inclusive of endpos+1
                    char ch = m_source[i++];
                    if (ch == '\n') {
                        ++m_normalized_line;
                        m_normalized_col = 1;
                    } else {
                        ++m_normalized_col;
                    }
                }
                m_pos = endpos + 2;
                return make_token(token_type::COMMENT_SY, val, comment_start, c_line, c_col);
            }
        }
    }
    if(match( "/")) return make_token(token_type::SLASH_SY, "/", start, start_line, start_col); 			// "/"
    if(match( ",")) return make_token(token_type::COMMA_SY, ",", start, start_line, start_col); 			// ","
    if(match( "R:")) return make_token(token_type::R_SY, "R:", start, start_line, start_col); 				// "R:"
    if(match( "Z")) return make_token(token_type::Z_SY, "Z", start, start_line, start_col); 				// "Z"
    if(match( "X")) return make_token(token_type::X_SY, "X", start, start_line, start_col); 				// "X"
    if(match( "Y")) return make_token(token_type::Y_SY, "Y", start, start_line, start_col); 				// "Y"
    if(match( "=")) return make_token(token_type::EQ_SY, "=", start, start_line, start_col);   				// "="
    if(match( "#")) return make_token(token_type::HASH_SY, "#", start, start_line, start_col);  			// "#"
    if(match( ";")) return make_token(token_type::SEMICOLON_SY, ";", start, start_line, start_col);// ";"
    if(std::isdigit(peek_char())) return number_token();

    /* nothing matched */
    m_last_error = "Unexpected character "     +
                   std::string{peek_char()} +
                   " at [line/col] ["          +
                   std::to_string(m_normalized_line) + "/"
                   + std::to_string(m_normalized_col);
    advance();
    return make_token(token_type::UNKNOWN, "", start, start_line, start_col);
}
