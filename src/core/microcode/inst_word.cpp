/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#include "inst_word.h"

#include <ostream>

using namespace core;

#define UNUSED(X) (void) X

/*
 * z->R0 z->R1 z->R_en */
#define Z_OFFSET_BITS 28
#define Z_WIDTH_BITS 4

 /* R0->x R1->x R2->x_en */
#define X_OFFSET_BITS 25
#define X_WIDTH_BITS 3

 /* R0->y R1->y R2->y_en */
#define Y_OFFSET_BITS 22
#define Y_WIDTH_BITS 3

 /* Op0 Op1 Op2 Op3 */
#define OP_OFFSET_BITS 18
#define OP_WIDTH_BITS 4

/* z->MAR z->MDR */
#define Z_MAR_OFFSET_BITS 17
#define Z_MAR_WIDTH_BITS 1

#define Z_MDR_OFFSET_BITS 16
#define Z_MDR_WIDTH_BITS 1

/* MDR->y MDR->cop */
#define MDR_Y_OFFSET_BITS 15
#define MDR_WIDTH_BITS 1

#define MDR_COP_OFFSET_BITS 14
#define MDR_WIDTH_BITS 1

 /* R_Mode0 R_Mode1 */
#define R_MODE_OFFSET_BITS 12
#define R_MODE_WIDTH_BITS 2

 /* AR_Mode0 AR_Mode1 */
#define AR_MODE_OFFSET_BITS 10
#define AR_MODE_WIDTH_BITS 2

/* Mask0 Mask1 Mask2 Mask3 */
#define MASK_OFFSET_BITS 6
#define MASK_WIDTH_BITS 4

 /* CN0 CN1 CN2 CN3 CN4 CN5 CN6 */
#define CN_OFFSET_BITS 0
#define CN_WIDTH_BITS 6

 /* = 32 bits total */

inst_word::inst_word()
    : msim_wrd{}
      , m_ram_mode{ram_mode::WAIT}
      , m_ar_mode{ar_mode::CHAR_PLS_PLS}
      , m_raw_word{0}
      , m_const_nbr{0}
{}


void inst_word::write_bits(uint8_t data, uint32_t offset_bits, uint32_t width_bits) {
    const uint32_t mask = ((1u << width_bits) - 1u) << offset_bits;
    m_raw_word = (m_raw_word & ~mask) | ((static_cast<uint32_t>(data) << offset_bits) & mask);
}

uint8_t inst_word::read_bits(uint32_t offset_bits, uint32_t width_bits) const {
    const uint32_t mask = (1u << width_bits) - 1u;
    return static_cast<uint8_t>((m_raw_word >> offset_bits) & mask);
}

uint32_t inst_word::get_raw_word() const {return m_raw_word;}


namespace core {
    std::ostream & operator<<(std::ostream & os, const inst_word & iw) {
        /* |b31|b30|b29|b28 | b27|b26|b25|b24 |b23|b22|b21|b20 | b19|b18|b17|b16 |b15|b14|b13|b12|b11|b10| b9| b8| b7| b6| b5| b4| b3| b2| b1| b0|*/
        /* | Z -> REGISTER | X SELECTION  |   Y SELECTION      |  OPERATION      |Z->MAR|Z->MDR|MDR->Y|MDR->COP| RAM MODE | AR MODE  |   MASK   |     CN      |*/
        std::string str{};
        for (int i = 0; i < 32; ++i) {
            bool bit_set = (iw.m_raw_word >> (31 - i)) & 1u;
            str += bit_set ? "1" : "0";
            if (i == 3 || i == 6 || i == 9 || i == 13 || i == 18 || i == 20 || i == 22 || i == 26) {
                str += " ";
            }
        }
        os << str << std::endl;
        return os;
    }

}

std::string inst_word::to_string() const {
    /* |b31|b30|b29|b28 | b27|b26|b25|b24 |b23|b22|b21|b20 | b19|b18|b17|b16 |b15|b14|b13|b12|b11|b10| b9| b8| b7| b6| b5| b4| b3| b2| b1| b0|*/
    /* | Z -> REGISTER | X SELECTION  |   Y SELECTION      |  OPERATION      |Z->MAR|Z->MDR|MDR->Y|MDR->COP| RAM MODE | AR MODE  |   MASK   |     CN      |*/
    std::string str{"|"};
    for (int i = 0; i < 32; ++i) {
        bool bit_set = (m_raw_word >> (31 - i)) & 1u;
        str += bit_set ? "1|" : "0|";
    }
    str += '\n';
    str += "|z->r0|r->x|r->y| op |z->mar|z->mdr|mdr->y|mdr->cop|ram mode|ar mode|  mask |     cn      |\n";
    return str;
}

void inst_word::set_z_selection(uint8_t z_nbl) {
    /* 0 0 0 0 R3 R2 R1 R0 */
    /* so Z->R0, Z->R1 etc.. is possible */
    uint8_t selection_translated = get_z_selection();
    switch (z_nbl) {
        case REGISTER_0: {
            selection_translated |= 0b00001000;
            break;
        }
        case REGISTER_1: {
            selection_translated |= 0b00000100;
            break;
        }
        case REGISTER_2: {
            selection_translated |= 0b00000010;
            break;
        }
        case REGISTER_3: {
            selection_translated |= 0b00000001;
            break;
        }
        default:
            add_error("invalid Z selection!");
            return;
    }
    write_bits(selection_translated, Z_OFFSET_BITS, Z_WIDTH_BITS);
}

void inst_word::set_x_selection(uint8_t x_nbl) {
    /* 0 0 0 0 0 s0 s1 en */
    write_bits(x_nbl, X_OFFSET_BITS, X_WIDTH_BITS);
}

void inst_word::set_y_selection(uint8_t y_nbl) {
    /* 0 0 0 0 0 s0 s1 en */
    write_bits(y_nbl, Y_OFFSET_BITS, Y_WIDTH_BITS);

}
void inst_word::set_operation(uint8_t operation) {
    /* 0 0 0 0 Op3 Op2 Op1 Op0 */
    write_bits(operation, OP_OFFSET_BITS, OP_WIDTH_BITS);

}
void inst_word::set_z_mar(bool z_mar) {
     write_bits(static_cast<uint8_t>(z_mar), Z_MAR_OFFSET_BITS, Z_MAR_WIDTH_BITS);
}

void inst_word::set_z_mdr(bool z_mdr) {
    write_bits(static_cast<uint8_t>(z_mdr),  Z_MDR_OFFSET_BITS, Z_MDR_WIDTH_BITS);
}
void inst_word::set_mdr_y(bool mdr_y ) {
    write_bits(static_cast<uint8_t>(mdr_y), MDR_Y_OFFSET_BITS, MDR_WIDTH_BITS);
}
void inst_word::set_mdr_cop(bool mdr_cop) {
    write_bits(static_cast<uint8_t>(mdr_cop), MDR_COP_OFFSET_BITS, MDR_WIDTH_BITS);
}

void inst_word::set_ram_mode(ram_mode mode) {
    uint8_t mode_nbl = 0;
    switch(mode){
        case ram_mode::WAIT:
            mode_nbl = 0b00;
            break;
        case ram_mode::READ:
            mode_nbl = 0b01;
            break;
        case ram_mode::WRITE:
            mode_nbl = 0b10;
            break;
        default:
            add_error("invalid ram mode set in instruction word!");
            return;
    }

    write_bits(mode_nbl,R_MODE_OFFSET_BITS, R_MODE_WIDTH_BITS);
}

void inst_word::set_ar_mode(ar_mode mode ) {
    uint8_t mode_nbl = 0;
    switch(mode){
        case ar_mode::CHAR_PLS_PLS:
            mode_nbl = 0b00;
            break;
        case ar_mode::_4COP:
            mode_nbl = 0b01;
            break;
        case ar_mode::_4COP_CND:
            mode_nbl = 0b10;
            break;
        case ar_mode::_4CN:
            mode_nbl = 0b11;
            break;
        default:
            add_error("invalid ar mode set in instruction word!");
            return;
    }
    write_bits(mode_nbl,AR_MODE_OFFSET_BITS, AR_MODE_WIDTH_BITS);
}

void inst_word::set_mask(uint8_t mask) {
    write_bits(mask, MASK_OFFSET_BITS, MASK_WIDTH_BITS);
}

void inst_word::set_cn(uint8_t cn) {
    write_bits(cn, CN_OFFSET_BITS, CN_WIDTH_BITS);
}

uint8_t inst_word::get_x_selection() const {
    return read_bits(X_OFFSET_BITS, X_WIDTH_BITS);
}
uint8_t inst_word::get_y_selection() const {
    return read_bits(Y_OFFSET_BITS, Y_WIDTH_BITS);
}
uint8_t inst_word::get_z_selection() const {
    return read_bits(Z_OFFSET_BITS, Z_WIDTH_BITS);
}
uint8_t inst_word::get_operation() const {
    return read_bits(OP_OFFSET_BITS, OP_WIDTH_BITS);
}
bool inst_word::get_z_mar() const {
    return read_bits(Z_MAR_OFFSET_BITS, Z_MAR_WIDTH_BITS) == 1;

}
bool inst_word::get_z_mdr() const {
    return read_bits(Z_MDR_OFFSET_BITS, Z_MDR_WIDTH_BITS) == 1;
}
bool inst_word::get_mdr_y() const {
    return read_bits(MDR_Y_OFFSET_BITS, MDR_WIDTH_BITS) == 1;

}
bool inst_word::get_mdr_cop() const {
    return read_bits(MDR_COP_OFFSET_BITS, MDR_WIDTH_BITS) == 1;
}

ram_mode inst_word::get_ram_mode() const{
    switch (read_bits(R_MODE_OFFSET_BITS, R_MODE_WIDTH_BITS)) {
        case 0b00:
            return ram_mode::WAIT;
        case 0b01:
            return ram_mode::READ;
        case 0b10:
            return ram_mode::WRITE;
        default:
            return ram_mode::WAIT; // return a safe default, never happens
    }
}

ar_mode inst_word::get_ar_mode() const {
    switch (read_bits(AR_MODE_OFFSET_BITS, AR_MODE_WIDTH_BITS)) {
        case 0:
            return ar_mode::CHAR_PLS_PLS;
        case 1:
            return ar_mode::_4COP;
        case 2:
            return ar_mode::_4COP_CND;
        case 3:
            return ar_mode::_4CN;
        default:
            return ar_mode::CHAR_PLS_PLS; // safe default, never happens
    }
}
uint8_t inst_word::get_mask() const {
    return read_bits(MASK_OFFSET_BITS, MASK_WIDTH_BITS);
}

int inst_word::get_cn() const {
    return read_bits(CN_OFFSET_BITS, CN_WIDTH_BITS);
}

int inst_word::get_constant_nbr() const {
    return m_const_nbr;
}

bool inst_word::ok() const {
    return true;
}

std::string inst_word::err_msg() {
    if (m_error_msgs.empty()) {
        return "";
    }
    std::string err_result = "{ ";
    for (std::size_t i = 0; i < m_error_msgs.size(); ++i) {
        err_result.append(m_error_msgs[i]);
        if (i + 1 < m_error_msgs.size()) {
            err_result.append(", ");
        }
    }
    err_result.append(" }");
    return err_result;
}
void inst_word::set_constant_nbr(int nbr) {
    if (m_const_nbr != 0) {
        add_error("Constant number already set in instruction word!");
        return;
    }
    m_const_nbr = nbr;
}

void inst_word::set_default_operation() {
    write_bits(Z_Z, OP_OFFSET_BITS, OP_WIDTH_BITS);
}
void inst_word::set_default_ar_opt() {
    /* ar_mode::CHAR_PLS_PLS ==> 0b00; */
    write_bits(0b00, AR_MODE_OFFSET_BITS, AR_MODE_WIDTH_BITS);
}
