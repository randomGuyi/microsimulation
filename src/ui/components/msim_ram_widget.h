/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */


#ifndef MSIM_RAM_WIDGET_H
#define MSIM_RAM_WIDGET_H

#include <QLabel>
#include <QMouseEvent>

#include "msim_component_svg_widget.h"

#include <shared/svg_loader.h>
#include <core/components/msim_ram.h>

namespace gui::components {
    class msim_ram_widget : public msim_component_svg_widget{
        Q_OBJECT
    public:
        msim_ram_widget(core::components::msim_ram * ram,
                        shared::svg_loader * loader,
                        QString const & element_id,
                        QWidget * parent = nullptr);

        void on_core_value_changed(int address_accessed);

        void update_display() override;

        void show_tooltip() override;
        void hide_tooltip() override;
    protected:
        core::components::msim_ram * m_ram;

        std::vector<QLabel*> m_value_labels;
        std::vector<QLabel*> m_addr_labels;

        // emit when the user clicks the placed component
    signals:
        void clicked(core::components::msim_ram * ram);

    protected:
        void mousePressEvent(QMouseEvent *event) override;

    };
}
#endif // MSIM_RAM_WIDGET_H
