#ifndef ADDRR_WORD_H
#define ADDRR_WORD_H

#include "msim_wrd.h"
#include <cstdint>


class addrr_word : public msim_wrd
{
public:
    addrr_word();
    void set_default();
    void set_cn(int cn = -1);
    void set_mask(uint8_t mask = 0);

    bool is_defalult();
    int get_cn();
    int get_mask();

    virtual void set_word(msim_wrd * wrd) override;
    virtual void set_or(msim_wrd * wrd) override;

private:
    int m_cn;
    uint8_t m_mask;

};

#endif // ADDRR_WORD_H
