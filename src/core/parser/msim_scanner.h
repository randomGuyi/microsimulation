#ifndef MSIM_SCANNER_H
#define MSIM_SCANNER_H

#include <cstddef>
#include <string>
#include <vector>



/* end of source */
#define EOS '\0'

    namespace core::parser {
        enum class token_type {
            HASH_SY,
            SLASH_SY,
            RAM_SY,
            READ_SY,
            WAIT_SY,
            WRITE_SY,
            H_SY,
            COMMA_SY,
            R0_TO_X_SY,
            R1_TO_X_SY,
            R2_TO_X_SY,
            R3_TO_X_SY,
            R0_TO_Y_SY,
            R1_TO_Y_SY,
            R2_TO_Y_SY,
            R3_TO_Y_SY,
            MDR_TO_COP_SY,
            MDR_TO_Y_SY,
            R_SY,
            Z_ASSIGN_SY,
            Z_SY,
            X_SY,
            Y_SY,
            INC_SY,
            DEC_SY,
            MIN_X_SY,
            PLS_Y_SY,
            MIN_Y_SY,
            TIMES_Y_SY,
            DIV_Y_SY,
            SHL_Y_SY,
            ASSIGN_CAR_SY,
            AR_SY,
            CAR_PLS_PLS_SY,
            _4_CN_SY,
            CN_ASSIGN_SY,
            _4COP_SY,
            IF_SY,
            COND_NE0_SY,
            MASKE_SY,
            EQ_SY,
            B_SY,
            Z_R0_SY,
            Z_R1_SY,
            Z_R2_SY,
            Z_R3_SY,
            Z_MAR_SY,
            Z_MDR_SY,
            NUMBER_SY,
            SEMICOLON_SY,
            EOF_SY,
            COMMENT_SY,
            UNKNOWN
        };

        struct token{
            token_type type;
            std::string value;
            int line;
            int column;
            size_t abs_pos; // needed for ignoring blanks, and coloring
        };


        class msim_scanner{

        public:
            msim_scanner(const std::string & source, bool ignore_comments);

            token get_next_token();
            token peek_next_token();
            [[nodiscard]] bool has_more_tokens() const;

            void reset(std::string const & text);
            [[nodiscard]] size_t get_position() const;
            void set_position(size_t pos);

            std::vector<token> get_tokens();
            std::vector<token> tokenize_line(size_t line_index);

            token get_token_at_position(int line, int col);

            [[nodiscard]] std::string get_last_error() const;

            int get_curr_line();
        private:
            size_t m_pos;
            std::string m_source;
            int m_normalized_line;
            int m_normalized_col;
            size_t m_normalize_offset;
            bool m_ignore_comments;

            std::string m_last_error;

            void skip_blanks();
            void skip_comment();
            token scan_token();
            bool match(const std::string & kw);
            token make_token(token_type tkt, const std::string & val, size_t start_pos, int start_line, int start_col);
            token number_token();
            [[nodiscard]] char peek_char() const;
            [[nodiscard]] char peek_next_char() const;
            char advance();
            bool is_at_end();
            std::string normalize_new_lines(const std::string & src);
        };
    }

#endif // MSIM_SCANNER_H
