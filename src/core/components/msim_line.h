#ifndef MSIM_LINE_H
#define MSIM_LINE_H

#include "msim_component.h"

namespace core::components {
    class msim_line : public msim_component
    {
    public:

        msim_line(std::string const & id, std::string const & label);
    };
}
#endif // MSIM_LINE_H
