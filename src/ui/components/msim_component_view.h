/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MSIM_COMPONENT_VIEW_H
#define MSIM_COMPONENT_VIEW_H

#include "ui/drag_and_drop/drop_target.h"
#include <QGraphicsProxyWidget>

namespace gui::components {
    class msim_component_view{
    public:
        virtual ~msim_component_view() = default;

        virtual QGraphicsProxyWidget * attach_to_target(drop_target * target) = 0;

        virtual void update_display() = 0;

        virtual void show_tooltip() = 0;
        virtual void hide_tooltip() = 0;

        virtual QString id() const = 0;
        virtual QString label() const = 0;
    };
}
#endif // MSIM_COMPONENT_VIEW_H
