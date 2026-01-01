//
// Created by Arthur on 15.11.25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_details_view.h" resolved

#include "details_view.h"
#include "ui_details_view.h"
#include <QVBoxLayout>

using namespace gui::views;

details_view::details_view(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::details_view)
    , m_tabs{new details_tabs{this}}
{
    ui->setupUi(this);
    // ensure there's a layout and add the tabs widget
    if (!this->layout()) {
        auto *l = new QVBoxLayout(this);
        l->setContentsMargins(0,0,0,0);
        l->addWidget(m_tabs);
    } else {
        this->layout()->addWidget(m_tabs);
    }
}

details_view::~details_view()
{
    delete ui;
}

void details_view::show_ram_details(core::components::msim_ram * instance){
    if(!instance) return;
    m_tabs->show_ram_details(instance);
}

void details_view::show_rom_details(core::components::msim_rom * instance){
    if(!instance) return;
    m_tabs->show_rom_details(instance);
}
