#ifndef INST_WORD_H
#define INST_WORD_H

#include <memory>

#include "msim_wrd.h"

enum class ram_mode{
    READ,
    WRITE,
    WAIT
};

enum class ar_mode {
   CHAR_PLS_PLS,
   _4COP,
   _4COP_CND,
   _4CN
};
/* selection commands */
/*                 s1, s0, en     */
#define REGISTER_0 0b00000001
#define REGISTER_1 0b00000011
#define REGISTER_2 0b00000101
#define REGISTER_3 0b00000111


/* decode commands */
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

class inst_word: public msim_wrd{
public:
    inst_word();

    void write_bits(uint8_t data, uint32_t offset_bits, uint32_t width_bits);

    uint8_t read_bits(uint32_t offset_bits, uint32_t width_bits);

    friend std::ostream &operator<<(std::ostream &os, const inst_word &iw);

    uint32_t get_raw_word();

    void set_x_selection(uint8_t x_nbl);
    void set_y_selection(uint8_t y_nbl);
    void set_z_selection(uint8_t z_nbl);


    void set_operation(uint8_t operation); // TODO: define Type 4 Operation
    void set_constant_nbr(int nbr);
    void set_default_operation();
    void set_z_mar(bool z_mar = false);
    void set_z_mdr(bool z_mdr = false);
    void set_mdr_y(bool mdr_y = false);
    void set_mdr_cop(bool mdr_cop = false);
    void set_ram_mode(ram_mode mode = ram_mode::WAIT);
    void set_ar_mode(ar_mode mode = ar_mode::CHAR_PLS_PLS);
    void set_default_ar_opt();
    void set_mask(uint8_t mask);
    void set_cn(uint8_t cn);

    uint8_t get_x_selection();
    uint8_t get_y_selection();
    uint8_t get_z_selection();
    uint8_t get_operation();
    bool get_z_mar();
    bool get_z_mdr();
    bool get_mdr_y();
    bool get_mdr_cop();
    ram_mode get_ram_mode();
    ar_mode get_ar_mode();
    uint8_t get_mask();
    int get_cn();

    virtual bool ok() const override;
    virtual std::string err_msg() override;


private:
    ram_mode m_ram_mode;
    ar_mode m_ar_mode;
    uint32_t m_raw_word;
    int m_const_nbr; // i know, it's cheating
};

#endif // INST_WORD_H
