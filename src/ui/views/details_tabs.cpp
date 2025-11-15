//
// Created by Arthur on 15.11.25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_details_tabs.h" resolved

#include "details_tabs.h"
#include "ui_details_tabs.h"


details_tabs::details_tabs(QWidget *parent)
    : QTabWidget{parent}
{
    setTabsClosable(true);
    setMovable(true);

    connect(this, &QTabWidget::tabCloseRequested, this, [this](int index) {
        QWidget *w = widget(index);
        removeTab(index);
        delete w;
    });
}
/* returns -1 if no tabs are found */
int details_tabs::tab_index_for_label(std::string const &lbl)
{
    for (int i = 0; i < count(); ++i) {
        if (tabText(i) == COMPILER_TAB_LABEL) {
            return i;
        }
    }
    return -1;
}

QTextEdit *details_tabs::get_or_make_compiler_log()
{
    int idx = tab_index_for_label(COMPILER_TAB_LABEL);
    QTextEdit *compiler_log = nullptr;

    /* check if a compiler tab already exists */
    if (idx == -1) {
        compiler_log = new QTextEdit;
        compiler_log->setReadOnly(true);
        addTab(compiler_log, COMPILER_TAB_LABEL);
    } else {
        compiler_log = qobject_cast<QTextEdit *>(widget(idx));
    }
    return compiler_log;
}

void details_tabs::show_ram_details(msim_ram *instance)
{
    if (instance == nullptr) {
        throw std::runtime_error{"details tabs got emty ram instance!"};
    }
}
void details_tabs::show_rom_details(msim_rom *instance)
{
    if (instance == nullptr) {
        throw std::runtime_error{"details tabs got emty rom instance!"};
    }
}

void details_tabs::show_compile_errors(std::vector<parser_error> const &psr_e)
{
    QTextEdit *compiler_log = get_or_make_compiler_log();
    compiler_log->clear();

    for (auto const &err : psr_e) {
        QString msg = QString("Line %1: %2").arg(err.line).arg(QString::fromStdString(err.message));
        compiler_log->append(msg);
    }

    setCurrentIndex(tab_index_for_label(COMPILER_TAB_LABEL));
}

void details_tabs::show_compile_details(std::string const &msg)
{
    QTextEdit *compiler_log = get_or_make_compiler_log();
    compiler_log->append(QString::fromStdString(msg));
    setCurrentIndex(tab_index_for_label(COMPILER_TAB_LABEL));
}