#ifndef MSIM_COP_WIDGET_H
#define MSIM_COP_WIDGET_H

#include "msim_component_svg_widget.h"

#include <shared/svg_loader.h>
#include <core/components/msim_cop.h>

namespace gui::components {
    class msim_cop_widget : public msim_component_svg_widget{
        Q_OBJECT
    public:
        msim_cop_widget(core::components::msim_cop * cop,
                        shared::svg_loader * loader,
                        QString const & element_id,
                        QWidget * parent = nullptr);

        virtual void update_display() override;

        virtual void show_tooltip() override;
        virtual void hide_tooltip() override;
    protected:
        core::components::msim_cop * m_cop;

    };
}
#endif // MSIM_COP_WIDGET_H
