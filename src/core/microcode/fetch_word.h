#ifndef FETCH_WORD_H
#define FETCH_WORD_H

#include "msim_wrd.h"
#include <cstdint>
#include <vector>


class fetch_word : public msim_wrd {
public:
    fetch_word();
    void set_mdr_sel(uint8_t mdr_sel_nbl);
    void set_y_sel(uint8_t y_sel_nbl);
    void set_x_sel(uint8_t x_sel_nbl);

    uint8_t get_mdr_sel();
    uint8_t get_y_sel();
    uint8_t get_x_sel();

    virtual void set_word(msim_wrd * wrd) override;
    virtual void set_or(msim_wrd * wrd) override;

private:
    uint8_t m_mdr_sel;
    uint8_t m_y_sel;
    uint8_t m_x_sel;
};

#endif // FETCH_WORD_H
