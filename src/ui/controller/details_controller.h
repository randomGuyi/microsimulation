#ifndef DETAILS_CONTROLLER_H
#define DETAILS_CONTROLLER_H

#include <QObject>

#include "ui/views/details_view.h"


namespace gui::views{
    class details_controller : public QObject {
        Q_OBJECT
    public:
        static details_controller & instance();

        void set_details_view(gui::views::details_view * view);

    public slots:
        void on_ram_clicked(core::components::msim_ram * ram);
        void on_rom_clicked(core::components::msim_rom * rom);

    private:
        details_controller();
        gui::views::details_view * m_view{nullptr};
    };
}

#endif // DETAILS_CONTROLLER_H

