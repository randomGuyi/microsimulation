#ifndef EXEC_WORD_H
#define EXEC_WORD_H

#include "msim_wrd.h"
#include <cstdint>


class exec_word : public msim_wrd{
public:
    exec_word();
    void set_nibble(uint8_t nbl);
    void set_or_nibble(uint8_t nbl);

    uint8_t get_nibble();

    virtual void set_word(msim_wrd * wrd) override;
    virtual void set_or(msim_wrd * wrd) override;
private:
    uint8_t m_nibble;
};

#endif // EXEC_WORD_H
