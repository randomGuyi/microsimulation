#ifndef COMPONENTS_PICKER_CONTROLLER_H
#define COMPONENTS_PICKER_CONTROLLER_H

#include "shared/component_info.h"

class components_picker_controller
{
public:
    components_picker_controller();
    QVector<component_info> get_all_components() const;
    QVector<component_info> get_registers() const;
    QVector<component_info> get_components() const;
};

#endif // COMPONENTS_PICKER_CONTROLLER_H
