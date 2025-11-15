#ifndef MSIM_WRD_H
#define MSIM_WRD_H

#include <string>
#include <vector>
class msim_wrd
{
public:
    msim_wrd();
    virtual bool ok() const;
    virtual std::string err_msg();

    virtual void set_word(msim_wrd * wrd) = 0;
    virtual void set_or(msim_wrd * wrd) = 0;
    virtual ~msim_wrd() = default;

protected:
    virtual void add_error(std::string const & error_msg);
    std::vector<std::string> m_error_msgs;
    bool m_ok;
};

#endif // MSIM_WRD_H
