/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */


#ifndef MICROSIMULATION_MSIM_MAIN_H
#define MICROSIMULATION_MSIM_MAIN_H

#include <QMainWindow> // <--- Use QMainWindow

QT_BEGIN_NAMESPACE


namespace Ui {class msim_main;}

QT_END_NAMESPACE

namespace gui::views {
    class msim_main : public QMainWindow {
        Q_OBJECT
    public:
        explicit msim_main(QWidget *parent = nullptr);
        ~msim_main() override;

    private:
        Ui::msim_main *ui;
    };
}
#endif