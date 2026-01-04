#ifndef MSIM_COMPONENT_H
#define MSIM_COMPONENT_H

#include <string>

namespace core::components {
    class msim_component{
    public:
        msim_component(std::string const & id, std::string const & label);
        virtual ~msim_component() = default;
        virtual void reset() = 0;
        virtual std::string id() const;
        virtual std::string label() const;

    protected:
        std::string m_id;
        std::string m_label;
    };
}
#endif // MSIM_COMPONENT_H
