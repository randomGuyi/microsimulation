//
// Created by Arthur on 15.11.25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_snippet_picker.h" resolved

#include "snippet_picker.h"
#include "ui_snippet_picker.h"


snippet_picker::snippet_picker(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::snippet_picker)
{
    ui->setupUi(this);
}

snippet_picker::~snippet_picker()
{
    delete ui;
}