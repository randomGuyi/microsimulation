#ifndef MSIM_COP_H
#define MSIM_COP_H


#include "msim_register.h"

class msim_cop : public msim_register{
public:
    msim_cop(std::string const & id, std::string const & label);
    
    void set_mask(int msk);
    void set_cn(int cn);
private:
    int m_mask;
    int m_cn;
};

#endif // MSIM_COP_H
