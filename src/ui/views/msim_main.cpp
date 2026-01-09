/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#include "msim_main.h"
#include <QMenuBar>
#include "ui_msim_main.h"
#include "details_view.h"
#include "../controller/details_controller.h"

using namespace gui::views;

msim_main::msim_main(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::msim_main)
{
    ui->setupUi(this);
    QMenu * menu = menuBar()->addMenu(tr("&Settings"));

    connect(ui->components_tab->get_auto_place_btn(), & QPushButton::clicked,
            this, [&](){
        auto drop_targets = ui->sim_frame->get_drop_targets();
        ui->components_tab->place_all_components(drop_targets);
    });

    // create and insert details view into the placeholder frame
    auto * details = new details_view{ui->details_frame};
    details->setParent(ui->details_frame);
    details->show();
    // register with controller
    gui::views::details_controller::instance().set_details_view(details);

}

msim_main::~msim_main()
{
    delete ui;
}
