#ifndef MSIM_SCANNER_H
#define MSIM_SCANNER_H

#include <cstddef>
#include <string>
#include <vector>



/* end of source */
#define EOS '\0'

/* gramma
pgm ::= (seg ";" inst )* ["\n"];

seg ::= ("#" number "/" number | number) ;

inst ::= [ramread ";"] [fetch ";"] [decode ";"] [execute ";"] [ramwrite ";"] ;

ramread ::= "RAM:" read ;
read ::= "lesen" | "read" | "[MAR]->MDR" ;

fetch ::= ("H:" | "F:") [register_to_x] [register_to_y] ["MDR" "->" "COP"] [","] ["MDR" "->" "Y"];
register_to_x ::= register "->" "X" [","];
register_to_y ::= register "->" "Y" [","];

register ::= "R0" | "R1" | "R2" | "R3") ;

decode ::= ("D:" | "R:") ([arop] | [ assign_z_op [ [","] arop]]) ;
assign_z_op ::= "Z" ":=" (noop | const | pass | calcop);
noop ::= "Z" ;
const ::= number ;
pass ::= "X" | "Y" ;
calcop ::= "X + Y"
         | "INC X"
         | "X++"
         | "INC Y"
         | "Y++"
         | "X * Y"
         | "X - Y"
         | "DEC X"
         | "X--"
         | "DEC Y"
         | "X << Y"
         | "X / Y"
         | "-X" ;

arop ::= "CAR++" | "CAR:=" ("4" ["*"] "COP" ["if" "cond" "<>" "0" ("MASKE=" | "MASK=" ) number]
                 | "4CN" "," "CN=" number ) ;

number ::= digit+ ;
digit ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;

execute ::= ("E:" | "B:") [ "Z" "->" register] [ "Z" "->" "MDR"] [ "Z" "->" "MAR"] ;

ramwrite ::= "RAM:" ("schreiben" | "write" | "wait" | "warten" | "MDR->[MAR]") ;

 */

    namespace core::parser {
        enum class token_type {
            HASH_SY,
            SLASH_SY,
            RAM_SY,

           // RAMReadOp    ::= "lesen" | "read" | "[MAR]->MDR" | "warten" | "wait" ;
            //LESEN_SY,
            READ_SY,
            //MAR_TO_MDR_SY,
            WAIT_SY,

        //    RAMWrite     ::= "RAM:" RAMWriteOp NewLine ;
            //RAMWriteOp   ::= "schreiben" | "write" | "MDR->[MAR]" ;
            //SCHREIB_SY,
            WRITE_SY,
            //MDR_TO_MAR_SY,

        //    HSec         ::= "H:" [HCmdList] NewLine ;
            H_SY,

        //    HCmdList     ::= HCmd ( "," HCmd )* ;           (* Komma-separierte Liste, mindestens 1 möglich *)
            COMMA_SY,
        //    HCmd         ::= XSel | YSel | MDROpt ;

        //    XSel         ::= "R0->X" | "R1->X" | "R2->X" | "R3->X" ;
            R0_TO_X_SY,
            R1_TO_X_SY,
            R2_TO_X_SY,
            R3_TO_X_SY,

        //YSel         ::= "R0->Y" | "R1->Y" | "R2->Y" | "R3->Y" ;

            R0_TO_Y_SY,
            R1_TO_Y_SY,
            R2_TO_Y_SY,
            R3_TO_Y_SY,

        // MDROpt       ::= "MDR->COP" | "MDR->Y" ;
            MDR_TO_COP_SY,
            MDR_TO_Y_SY,

        //    (* Decode / Rechenphase *)
        //    RSec         ::= "R:" [RCmdList] NewLine ;
            R_SY,

        //    RCmdList     ::= RCmd ( "," RCmd )* ;
        //    RCmd         ::= "Z:=" RExpr | ARsec   ;
            Z_ASSIGN_SY,

        //    RExpr        ::= "Z"
        //               | Number
        //               | "X" XTail
        //               | "Y"
        //               | "INC" ("X" | "Y")
        //               | "DEC" ("X" | "Y")
        //               | "-X" ;
            Z_SY,
            X_SY,
            Y_SY,
            INC_SY,
            DEC_SY,
            MIN_X_SY,

        //    XTail        ::= "+Y" | "-Y" | "*Y" | "/Y" | "«Y" | ε ;
            PLS_Y_SY,
            MIN_Y_SY,
            TIMES_Y_SY,
            DIV_Y_SY,
            SHL_Y_SY,


           // (* Address / CAR ops *)
            //ARSec        ::= "CAR:=" ARAssign NewLine
            ASSIGN_CAR_SY,
            //      | "AR:" AROp NewLine ;   (* optional syntax variants *)
            AR_SY,
        //    AROp         ::= "CAR++" | "CAR:=" ARAssign ;
            CAR_PLS_PLS_SY,


           // ARAssign     ::= "4CN" [ "," "CN=" Number ]
            _4_CN_SY,
            CN_ASSIGN_SY,
        //                  | "4COP" [ "IF" "cond<>0" "MASKE" "=" Number ] ;
            _4COP_SY,
            IF_SY,
            COND_NE0_SY,
            MASKE_SY,
            EQ_SY,

        //    (* Bringphase *)
        //    BSec         ::= "B:" [ BringList ] NewLine ;
            B_SY,

         //   BringList    ::= BringOp ( "," BringOp )* ;
        //   BringOp      ::= "Z->R0" | "Z->R1" | "Z->R2" | "Z->R3"
        //                 | "Z->MAR" | "Z->MDR" ;
            Z_R0_SY,
            Z_R1_SY,
            Z_R2_SY,
            Z_R3_SY,
            Z_MAR_SY,
            Z_MDR_SY,


        //    (* Helper *)
         //   Number       ::= Digit+ ;
        //    Digit        ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' ;
            NUMBER_SY,
        //    NewLine      ::= ';' ;
              SEMICOLON_SY,
        //    EOF          ::= <end-of-file> ;
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
