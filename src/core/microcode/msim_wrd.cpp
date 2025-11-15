#include "msim_wrd.h"

msim_wrd::msim_wrd()
: m_ok{true}
{}

std::string msim_wrd::err_msg() {
    std::string err_result;
    if(! ok()){
        err_result.append("{ ");
        for(std::size_t i = 0; i < m_error_msgs.size(); ++i){
            err_result.append(m_error_msgs[i]);
            if(i + 1 < m_error_msgs.size()){
                err_result.append(", ");
            }
        }
        err_result.append(" }");
    }
    return err_result;
}

void msim_wrd::add_error(std::string const & error_msg){
    m_ok = false;
    m_error_msgs.push_back(error_msg);
}

bool msim_wrd::ok() const{
    return m_ok;
}
