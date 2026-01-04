#ifndef MSIM_BUS_WIDGET_H
#define MSIM_BUS_WIDGET_H

#include "core/components/msim_bus.h"
#include "ui/components/msim_component_svg_widget.h"
namespace gui::components {
    class msim_bus_widget :public msim_component_svg_widget{
        Q_OBJECT
    public:
        msim_bus_widget(core::components::msim_bus * bus,
                        shared::svg_loader * loader,
                        QString const & element_id,
                        QWidget * parent = nullptr);

        virtual void update_display() override;

        virtual void show_tooltip() override;
        virtual void hide_tooltip() override;

    private:
        core::components::msim_bus * m_bus;
    };
}
#endif // MSIM_BUS_WIDGET_H
