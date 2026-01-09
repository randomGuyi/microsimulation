/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#ifndef COMPONENTS_PICKER_CONTROLLER_H
#define COMPONENTS_PICKER_CONTROLLER_H

#include "shared/component_info.h"

namespace gui::views {
    class components_picker_controller
    {
    public:
        components_picker_controller();
        QVector<shared::component_info> get_all_components() const;
        QVector<shared::component_info> get_registers() const;
        QVector<shared::component_info> get_components() const;
    };
}

#endif // COMPONENTS_PICKER_CONTROLLER_H
