#ifndef INST_WORD_H
#define INST_WORD_H

#include <memory>

#include "addrr_word.h"
#include "decode_word.h"
#include "exec_word.h"
#include "msim_wrd.h"

#include "fetch_word.h"

enum class ram_mode{
    READ,
    WRITE,
    WAIT
};

class inst_word: public msim_wrd{
public:
    inst_word();
    void set_read(bool read);
    void set_fetch_ops(fetch_word & fw);
    void set_decode_ops(decode_word & dw);
    void set_ar_ops(addrr_word & aw);
    void set_exec_ops(exec_word & ew);
    void set_write(bool);

    bool get_read() const ;
    bool get_write() const;
    fetch_word * get_fetch_ops() const;
    decode_word * get_decode_ops() const;
    addrr_word * get_ar_ops() const ;
    exec_word * get_exec_ops()const;


    virtual void set_word(msim_wrd * wrd) override;
    virtual void set_or(msim_wrd * wrd) override;

    virtual bool ok() const override;
    virtual std::string err_msg() override;

private:
    std::unique_ptr<fetch_word> m_fetch_wrd;
    std::unique_ptr<decode_word> m_decode_wrd;
    std::unique_ptr<addrr_word> m_addrr_wrd;
    std::unique_ptr<exec_word> m_exec_wrd;

    ram_mode m_ram_mode;

};

#endif // INST_WORD_H
