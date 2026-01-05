//
// Created by Arthur on 15.11.25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_snippet_picker.h" resolved

#include "snippet_picker.h"

#include <QTextEdit>

#include "ui_snippet_picker.h"

using namespace gui::views;
snippet_picker::snippet_picker(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::snippet_picker)
{
    ui->setupUi(this);
    QTextEdit *edit = new QTextEdit(this);
    edit->setPlainText("// Snippet Picker\n// Select a code snippet from the list below:\n\n");
}

snippet_picker::~snippet_picker()
{
    delete ui;
}