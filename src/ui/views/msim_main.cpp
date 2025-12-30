//
// Created by Arthur on 15.11.25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_msim_main.h" resolved

#include "msim_main.h"
#include <QMenuBar>
#include "ui_msim_main.h"

using namespace gui::views;

msim_main::msim_main(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::msim_main)
{
    ui->setupUi(this);
    QMenu * menu = menuBar()->addMenu(tr("&Settings"));
    QAction * auto_place_action = new QAction(tr("&auto place"), this);

    menu->addAction(auto_place_action);
    connect(auto_place_action, & QAction::triggered,
            this, [&](){
        auto drop_targets = ui->sim_frame->get_drop_targets();
        ui->components_tab->place_all_components(drop_targets);
    });


}

msim_main::~msim_main()
{
    delete ui;
}

