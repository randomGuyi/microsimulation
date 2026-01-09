/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MSIM_CONNECTOR_WIDGET_H
#define MSIM_CONNECTOR_WIDGET_H

#include "core/components/msim_connector.h"
#include "ui/components/msim_component_svg_widget.h"
#include "ui/components/msim_line_widget.h"

#define NUMBER_OF_CONNECTIONS 4 // in, out, phase, en
namespace gui::components {
    class msim_connector_widget : public msim_component_svg_widget
    {
        Q_OBJECT
    public:
        explicit msim_connector_widget(core::components::msim_connector * con,
                                       shared::svg_loader * loader,
                                       QString const & element_id,
                                       QWidget * parent = nullptr);

        void on_core_value_changed(bool value);
        void add_line(msim_line_widget * line);
        void update_display() override;
        void show_tooltip() override;
        void hide_tooltip() override;
    protected:
        core::components::msim_connector * m_connector;
        QList<msim_line_widget *> m_connections;

    };
}
#endif // MSIM_CONNECTOR_WIDGET_H
