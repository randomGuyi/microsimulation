#ifndef SIMULATOR_CONTROLLER_H
#define SIMULATOR_CONTROLLER_H

#include "ui/drag_and_drop/drop_target.h"

namespace gui::views {
    class simulator_controller
    {
    public:
        explicit simulator_controller();
        void register_drop_target(drop_target * target);
        void handle_component_drop(QString const & id, QString const & label);
    };
}
#endif // SIMULATOR_CONTROLLER_H
