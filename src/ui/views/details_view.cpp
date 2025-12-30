//
// Created by Arthur on 15.11.25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_details_view.h" resolved

#include "details_view.h"
#include "ui_details_view.h"

using namespace gui::views;

details_view::details_view(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::details_view)
    , m_tabs{new details_tabs{this}}
{
    ui->setupUi(this);
}

details_view::~details_view()
{
    delete ui;
}