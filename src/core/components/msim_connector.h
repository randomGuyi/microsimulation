#ifndef MSIM_CONNECTOR_H
#define MSIM_CONNECTOR_H

#include "msim_observable_component.h"
#include "core/components/msim_component.h"
namespace core::components {
    class msim_connector : public msim_component,
                           public msim_observable_component<bool>
    {
    public:
        msim_connector(std::string const & id, std::string const & label);
        void enable();

        void disable();
        bool is_enabled();
        void reset() override {
            m_enabled = false;
            notify(m_enabled);
        }

    private:
        bool m_enabled;

    };
}
#endif // MSIM_CONNECTOR_H
