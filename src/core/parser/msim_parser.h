#ifndef MSIM_PARSER_H
#define MSIM_PARSER_H

#include "../microcode/addrr_word.h"
#include "../microcode/decode_word.h"
#include "../microcode/exec_word.h"
#include "../microcode/fetch_word.h"
#include "../microcode/inst_word.h"
#include "msim_scanner.h"
#include "../../core/components/msim_rom.h"

/*
```ebnf
Program      ::=
{                               (*sem*) inst_word w{}; (*end sem*)
Segment O_line IO_inst_word     (*sem*) rom.load_inst(inst_word, line) (*end sem*)
} EOF ;

Segment O_line IO_inst_word  ::=(*sem*) bool seg_set{false}; int seg{0}; (*end sem*)
[ '#' Number ^n                 (*sem*) seg = n; seg_set = true;(*end sem*)
'/' ]
Number^n                  (*sem*) line = n;
                                    if(seg_set && ! check_segment(line, seg)){
                                        std::cerr << .....
                                    }
                                (*end sem*)
";"
InstructionBlock O_word         (*sem*) inst_word = word (*end sem*)
;

InstructionBlock O_word::=      (*sem*) inst_word w{};
                                        bool read_set{false};
                                (*end sem*)
[ RAMRead O_r_opt               (*sem*) w.set_read(r);
                                        read_set = true;
                                (*end sem*)
";"
][FetchSec O_h_word                 (*sem*) w.set_fetch_ops(h_word); (*end sem*)
";"
][DecodeSec O_r_word O_ar_word       (*sem*) w.set_read_ops(r_word);
                                        w.set_ar_ops(ar_word);
                                (*end sem*)
";"
][ExecSec O_b_word                 (*sem*) w.set_exec_ops(b_word);  (*end sem*)
";"
][ RAMWrite O_w_opt             (*sem*) if(read_set){
                                            std::cerr << "already in mode read! at line: " << editor.get_scanner.get_line() << std::endl;
                                        }else{
                                            w.set_write(w_opt);
                                        }
                                (*end sem*)
";"
]                               (*sem*) word = w;              (*end sem*)
;

RAMRead O_r_opt ::= "RAM:"
RAMReadOp O_read                (*sem*) r_opt = read; (*endsem*)
;

RAMReadOp O_read  ::= "lesen"   (*sem*) read = true;  (*end sem*)
               | "read"         (*sem*) read = true;  (*end sem*)
               | "[MAR]->MDR"   (*sem*) read = true;  (*end sem*)
               | "warten"       (*sem*) read = true;  (*end sem*)
               | "wait" ;       (*sem*) read = true;  (*end sem*)

RAMWrite O_w_opt ::= "RAM:"
RAMWriteOp O_write              (*sem*) w_opt = write; (*end sem*)
;

RAMWriteOp O_write::= "schreiben"(*sem*) write = true;   (*end sem*)
               | "write"        (*sem*) write = true;   (*end sem*)
               | "MDR->[MAR]"   (*sem*) write = true;   (*end sem*)
               | "empty         (*sem*) write = false;   (*end sem*)
               ;

FetchSec O_fetch_word::= "H:"       (*sem*) fetch_word w{}; (*end sem*)
                 [FetchCmdList O_fetch_wrd
                                (*sem*) w.set(fetch_wrd); (*end sem*)
                 ]              (*sem*) h_word = w;      (*end sem*)
                 ;

FetchCmdList O_fetch_wrd ::=       (*sem*) fetch_word w{};      (*end sem*)
FetchCmd O_fetch_cmd                (*sem*) w.set_or(fetch_cmd); (*end sem*)
{ "," FetchCmd O_fetch_cmd          (*sem*) w.set_or(fetch_cmd);
                                        if(! w.ok()){
                                                std::cerr << "Fetch error line: " << editor.get_scanner().get_line() <<
                                                              " msg: " << w.get_err_msg();
                                        }
                                 (*end sem*)
};

FetchCmd O_fetch_cmd ::=     (*sem*) fetch_word_w{};        (*end sem*)
XSel O_x_nbl                 (*sem*) w.set_x_sel(x_nbl);    (*end sem*)
| YSel O_y_nbl               (*sem*) w.set_y_sel(y_nbl);    (*end sem*)
| MDROpt O_mdr_nbl           (*sem*) w.set_mdr_sel(mdr_nbl) (*end sem*)
;

XSel O_x_nbl ::=           (*sem*) byte bt{0};     (*end sem*)
"R0->X"                    (*sem*) bt |= (1 << 3); (*endsem*)
| "R1->X"                  (*sem*) bt |= (1 << 2); (*endsem*)
| "R2->X"                  (*sem*) bt |= (1 << 1); (*endsem*)
| "R3->X"                  (*sem*) bt |= (1 << 0);
                                   x_nbl = bt;
                           (*endsem*)
;

YSel O_y_nbl ::=           (*sem*) byte bt{0};     (*end sem*)
"R0->Y"                    (*sem*) bt |= (1 << 3); (*endsem*)
| "R1->Y"                  (*sem*) bt |= (1 << 2); (*endsem*)
| "R2->Y"                  (*sem*) bt |= (1 << 1); (*endsem*)
| "R3->Y"                  (*sem*) bt |= (1 << 0);
                                   y_nbl = bt;
                           (*endsem*)
;

MDROpt O_mdr_nbl::=  (*sem*) byte bt{0} (*end sem*)
"MDR->COP"           (*sem*) bt |= (1 << 1); (*endsem*)
| "MDR->Y"           (*sem*) bt |= (1 << 0);
                             mdr_nbl = bt;
                     (*endsem*)
;

(* Decode / Rechenphase *)
DecodeSec O_dec_word O_ar_word ::=
"R:" [DecodeCmdList O_dec_cmd O_ar_cmd (*sem*) O_dec_word = dec_cmd;
                                        ar_word = ar_cmd;
                                (*endsem*)
] ;

DecodeCmdList O_r_cmd O_ar_cmd::=    (*sem*) decode_word dw{};
                                        ar_word aw{};
                                (*endsem*)
DecodeCmd O_dec_word O_ar_word       (*sem*) dw.set_or(dec_word);
                                        aw.set_or(ar_word);
                                        if(! aw.ok()){
                                           std::cerr << "Error at line {" << scanner.get_line() << "} :" aw.get_err_msg() << std::end;
                                        }
                                        if(! dw.ok()){
                                           std::cerr << "Error at line {" << scanner.get_line() << "} :" dw.get_err_msg() << std::end;
                                        }
                                (*end sem*)

{ "," DecodeCmd O_dec_word O_ar_word (*sem*) dw.set_or(dec_word);
                                        aw.set_or(ar_word);
                                        if(! aw.ok()){
                                           std::cerr << "Error at line {" << scanner.get_line() << "} :" aw.get_err_msg() << std::end;
                                        }
                                        if(! dw.ok()){
                                           std::cerr << "Error at line {" << scanner.get_line() << "} :" dw.get_err_msg() << std::end;
                                        }
                                (*end sem*)
 }  ;

DecodeCmd O_dec_word O_ar_word ::= "Z:="(*sem*) decode_word dw{};
                                           ar_word aw{};
                                   (*end sem*)

DecodeExpr O_r_cmd                      (*sem*) dw = r_cmd; (*end sem*)
| ARSec O_ar_cmd                   (*sem*) aw = ar_cmd;(*end sem*)
                                   (*sem*) dec_word = dw; (*end sem*)
                                   (*sem*) ar_word = aw; (*end sem*)
;
(*  there will be constants for the operations:
* ----
* Z_Z          --> 0 0 0 0
* Z_CONST      --> 0 0 0 1
* Z_X          --> 0 0 1 0
* Z_Y          --> 0 0 1 1
* Z_X_PLS_Y    --> 0 1 0 0
* Z_INC_X      --> 0 1 0 1
* Z_INC_Y      --> 0 1 1 0
* Z_X_TIMES_Y  --> 0 1 1 1
* Z_X_MINUS_Y  --> 1 0 0 0
* Z_DEC_X      --> 1 0 0 1
* Z_DEC_Y      --> 1 0 1 0
* Z_X_DIV_Y    --> 1 0 1 1
* Z_X_SHL_Y    --> 1 1 0 0
* Z_MINUS_X    --> 1 1 0 1
*)

DecodeExpr O_r_cmd ::= "Z:=" (*sem*) decode_word dw{}; (*end sem*)

( Number n        (*sem*) dw.set_const(n) (*end sem*)
| "X"			  (*sem*) byte tail{0}; (*end sem*)
    [XTail O_t   (*sem*) byte tail{O_t}; (*end sem*)
]                 (*sem*) if(tail == 0){
                            dw.set_byte(Z_X);
                          }else{
                            dw.set_byte(t);
                        }
                  (*end_sem*)
| "Y"             (*sem*) dw.set_const(Z_Y); (*end sem*)
| "INC" ("X"      (*sem*) dw.set_const(Z_INC_X); (*end sem*)
        |"Y"      (*sem*) dw.set_const(Z_INC_Y); (*end sem*)
        )
| "DEC" ("X"      (*sem*) dw.set_const(Z_DEC_X); (*end sem*)
        | "Y"     (*sem*) dw.set_const(Z_DEC_Y); (*end sem*)
        )
| "-X"            (*sem*) dw.set_const(Z_MINUS_X) (*end sem*)
| "Z"             (*sem*) dw.set_default (*end sem*)
)
;

XTail O_t ::=
"+Y"              (*sem*) t = Z_X_PLS_Y; (*end sem*)
| "-Y"            (*sem*) t = Z_X_MINUS_Y; (*end sem*)
| "*Y"            (*sem*) t = Z_X_TIMES_Y; (*end sem*)
| "/Y"            (*sem*) t = Z_X_DIV_Y; (*end sem*)
| "«Y"            (*sem*) t = Z_X_SHL_Y; (*end sem*)
;

(* Address / CAR ops *)
ARSec O_ar_cmd ::=              (*sem*) ar_word aw{}; (*end sem*)
("CAR:=" ARAssign O_ar_ass      (*sem*) aw.set_word(ar_ass); (*end sem*)
| "CAR++"             (*sem*) aw.set_default(); (*endsem*)
) 								(*sem*)
                                       ar_cmd = aw;
                                (*end sem*)
;

ARAssign O_ar_ass    ::= ("4CN"       (*sem*) int _cn = -1;
                                             ar_word aw{};
                                     (*end sem*)
[ "," "CN=" Number                   (*sem*) _cn = n;      (*end sem*)
]                                    (*sem*) (_cn > 0){
                                                aw.set_cn(_cn);
                                             }else{
                                                aw.set_cn();
                                             }
                                     (*end sem*)
| "4COP"                             (*sem*) int mask = -1; (*end sem*)
[ "IF" "cond<>0" "MASKE" "=" Number  (*sem*) mask = n; (*end sem*)
])                                    (*sem*) if(mask > 0){
                                                aw.set_cop(mask);
                                             }else{
                                                aw.set_cop();
                                             }
                                             ar_ass = aw;
                                     (*end sem*)
;

(* Bringphase *)
ExecSec O_b_word::= "B:"        (*sem*) exec_word w{}; (*end sem*)
[ ExecList O_exec_opt       	(*sem*) w = exec_opt;(*end sem*)
] 							    (*sem*) b_word = w;  (*end sem*)
;

ExecList O_exec_opt ::=     (*sem*) exec_word w{}; (*end sem*)
ExecOp O_exec_nibble        (*sem*) w.set_nibble(exec_nibble); (*end sem*)
 ( "," BringOp O_exec_nibble (*sem*) w.set_or(exec_nibble);
                                     if(! w.ok()){
                                        std::cerr << "Exec error at line: " << editor.get_scanner().get_line() <<
                                                     " msg: " << w.get_err_msg;
                                     }
                             (*end sem*)
 )*
 ;
ExecOp O_exec_nibble ::=   (*sem*) byte bt{0}; (*end sem*)
 "Z->R0"                    (*sem*) bt |= (1 << 5); (*endsem*)
 | "Z->R1"                  (*sem*) bt |= (1 << 4); (*endsem*)
 | "Z->R2"                  (*sem*) bt |= (1 << 3); (*endsem*)
 | "Z->R3"                  (*sem*) bt |= (1 << 2); (*endsem*)
 | "Z->MAR"                 (*sem*) bt |= (1 << 1); (*endsem*)
 | "Z->MDR"                 (*sem*) bt |= (1 << 0); (*endsem*)
                            (*sem*) exec_nibble = bt; (*endsem*)
 ;

(* Helper *)
Number       ::= Digit+ ;
Digit        ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' ;
NewLine      ::= '\n' ;
EOF          ::= <end-of-file> ;

```
*/

struct parser_error{
    int line;
    std::string message;
};

class msim_parser
{
public:
    msim_parser(msim_scanner * scanner_inst, msim_rom * rom_inst);
    void parse();
    bool has_error() const;
    std::vector<parser_error> get_errors();
    void set_rom_inst(msim_rom * inst);
    void set_scanner_inst(msim_scanner * inst);

private:

    void program();
    void segment(int & line, inst_word & iw);
    void instruction_block(inst_word & word);

    void ram_read( bool & r_opt);
    void ram_read_op(bool & read);

    void ram_write(bool & w_opt);
    void ram_write_opt(bool & write);

    void fetch_sec(inst_word & f_word);
    void fetch_cmd_list(fetch_word & fetch_wrd);
    void fetch_cmd (fetch_word & fetch_c);

    void x_sel(uint8_t & x_nbl);
    void y_sel(uint8_t & y_nbl);
    void mdr_opt(uint8_t & mdr_nbl);

    void decode_sec(decode_word & dec_word, addrr_word & ar_wrd );
    void decode_cmd_list( decode_word & dec_cmd, addrr_word & ar_cmd);
    void decode_cmd(decode_word & dec_word, addrr_word & ar_word);

    void decode_expr (decode_word & dec_cmd);

    void x_tail(uint8_t & t);
    void ar_sec(addrr_word & ar_cmd);
    void ar_assign (addrr_word & ar_ass);

    void exec_sec( exec_word & ex_wrd);
    void exec_list (exec_word & exec_opt);
    void exec_op (uint8_t & exec_nibble);

    void number(int & nbr);
    void binary_number(uint8_t & digit);

    void next_token();
    void expect(token_type expected);
    token peek_next_token();

    void syntax_error(const std::string& expected);
    void semantic_error(const std::string& msg);
    void panic_mode_recovery();

    void emit_instruction(inst_word & w, int line);
    bool check_segment(int seg, int line);

    void set_bit(uint8_t &byte, int pos);

    msim_scanner * m_scanner;
    msim_rom * m_rom;
    token m_lookahead;

    std::vector<parser_error> m_errors;

};

#endif // MSIM_PARSER_H
