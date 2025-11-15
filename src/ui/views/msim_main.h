//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_MSIM_MAIN_H
#define MICROSIMULATION_MSIM_MAIN_H

#include <QMainWindow> // <--- Use QMainWindow

QT_BEGIN_NAMESPACE

namespace Ui {
    class msim_main;
}

QT_END_NAMESPACE

// CHANGE: Inherit from QMainWindow
class msim_main : public QMainWindow {
    Q_OBJECT
public:
    // QMainWindow destructor is not override, but it's safe to keep 'override' if you must
    explicit msim_main(QWidget *parent = nullptr);
    msim_main();
    ~msim_main() override;

private:
    Ui::msim_main *ui;

};
#endif