#ifndef DECODE_WORD_H
#define DECODE_WORD_H

#include "msim_wrd.h"
#include <cstdint>
#include <iostream>

#define Z_Z          0b00000000
#define Z_CONST      0b00000001
#define Z_X          0b00000010
#define Z_Y          0b00000011
#define Z_X_PLS_Y    0b00000100
#define Z_INC_X      0b00000101
#define Z_INC_Y      0b00000110
#define Z_X_TIMES_Y  0b00000111
#define Z_X_MINUS_Y  0b00001000
#define Z_DEC_X      0b00001001
#define Z_DEC_Y      0b00001010
#define Z_X_DIV_Y    0b00001011
#define Z_X_SHL_Y    0b00001100
#define Z_MINUS_X    0b00001101

class decode_word : public msim_wrd{
public:
    decode_word();
    void set_const_number(int const_n);
    int get_const_number();

    void set_operation(uint8_t operation);
    uint8_t get_operation();

    void set_default();

//    void set_const(uint8_t const_n);
 //    void set_byte(uint8_t const_n);

//    uint8_t get_const();
 //   uint8_t get_byte();

    virtual void set_word(msim_wrd * wrd) override;
    virtual void set_or(msim_wrd * wrd) override;

private:
    int m_constant;
    uint8_t m_operation;

};

#endif // DECODE_WORD_H
