/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#ifndef MICROSIMULATION_DETAILS_VIEW_H
#define MICROSIMULATION_DETAILS_VIEW_H

#include <QWidget>
#include "details_tabs.h"

QT_BEGIN_NAMESPACE

namespace Ui {class details_view;}

QT_END_NAMESPACE
namespace gui::views {
    class details_view : public QWidget {
        Q_OBJECT

    public:
        explicit details_view(QWidget *parent = nullptr);

        ~details_view() override;
    public slots:
       void show_ram_details(core::components::msim_ram * instance);
       void show_rom_details(core::components::msim_rom * instance);

    private:
        Ui::details_view *ui;
        details_tabs * m_tabs;
    };
}

#endif //MICROSIMULATION_DETAILS_VIEW_H