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

class inst_word: public msim_wrd{
public:
    inst_word();

    void write_bits(uint8_t data, uint32_t offset_bits, uint32_t width_bits);

    uint32_t get_raw_word();

    void set_x_selection(uint8_t x_nbl);
    void set_y_selection(uint8_t y_nbl);
    void set_z_selection(uint8_t z_nbl);
    void set_operation(uint8_t operation); // TODO: define Type 4 Operation
    void set_z_mar(bool z_mar = false);
    void set_z_mdr(bool z_mdr = false);
    void set_mdr_y(bool mdr_y = false);
    void set_mdr_cop(bool mdr_cop = false);
    void set_ram_mode(ram_mode mode = ram_mode::WAIT);
    void set_ar_mode(ar_mode mode = ar_mode::CHAR_PLS_PLS);
    void set_mask(uint8_t mask);
    void set_cn(int cn);

    uint8_t get_x_selection();
    uint8_t get_y_selection();
    uint8_t get_z_selection();
    uint8_t get_operation();
    bool get_z_mar();
    bool get_z_mdr();
    bool get_mdr_y();
    ar_mode get_mdr_cop();
    ram_mode get_ram_mode();
    ar_mode get_ar_mode();
    uint8_t get_mask();
    int get_cn();

    virtual void set_word(msim_wrd * wrd) override;
    virtual void set_or(msim_wrd * wrd) override;

    virtual bool ok() const override;
    virtual std::string err_msg() override;

private:
    ram_mode m_ram_mode;
    ar_mode m_ar_mode;
    uint32_t m_raw_word;
};

#endif // INST_WORD_H
