//
// Created by Arthur on 15.11.25.
//

#include "details_tabs.h"
#include <QString>
#include <QChar>
#include <cctype>

using namespace gui::views;
details_tabs::details_tabs(QWidget *parent)
    : QTabWidget{parent}
{
    setTabsClosable(true);
    setMovable(true);

    connect(this, &QTabWidget::tabCloseRequested, this, [this](int index) {
        QWidget *w = widget(index);
        // if the closed widget is our cached RAM text view, clear the pointer to avoid a dangling pointer
        if (w == m_ram_text_view) {
            m_ram_text_view = nullptr;
        }
        removeTab(index);
        delete w;
        w = nullptr;
    });
}
/* returns -1 if no tabs are found */
int details_tabs::tab_index_for_label(std::string const &lbl)
{
    const QString qlbl = QString::fromStdString(lbl);
    for (int i = 0; i < count(); ++i) {
        if (tabText(i) == qlbl) {
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

void details_tabs::show_ram_details(core::components::msim_ram *instance)
{
    if (instance == nullptr) {
        throw std::runtime_error{"details tabs got emty ram instance!"};
    }

    QFont font("Courier", 18);
    // create text view if necessary
    if (!m_ram_text_view) {
        m_ram_text_view = new QTextEdit;
        m_ram_text_view->setFont(font);
        m_ram_text_view->setReadOnly(true);
        addTab(m_ram_text_view, RAM_TAB_LABEL);
    }

    m_ram_text_view->clear();
    auto const & data = instance->get_ram_data();
    if (data.empty()) {
        m_ram_text_view->append(QStringLiteral("<i>RAM is empty</i>"));
    } else {
        for (auto const &p : data) {
            int addr = p.first;
            int val = p.second;
            char ch = (std::isprint(static_cast<unsigned char>(val)) ? static_cast<char>(val) : '.');
            QString line = QString("%1: 0x%2 '%3'").arg(addr).arg(val, 2, 16, QChar('0')).arg(QChar(ch));
            m_ram_text_view->append(line);
        }
    }
    // switch to ram tab
    int idx = indexOf(m_ram_text_view);
    if (idx != -1) setCurrentIndex(idx);
}
void details_tabs::show_rom_details(core::components::msim_rom *instance)
{
    if (instance == nullptr) {
        throw std::runtime_error{"details tabs got emty rom instance!"};
    }
    int idx = tab_index_for_label(ROM_TAB_LABEL);
    QTextEdit * rom_text = nullptr;
    if (idx == -1) {
        QFont font("Courier", 18);
        rom_text = new QTextEdit;
        rom_text->setFont(font);
        rom_text->setReadOnly(true);
        addTab(rom_text, ROM_TAB_LABEL);
    } else {
        rom_text = qobject_cast<QTextEdit *>(widget(idx));
    }

    rom_text->clear();
    auto const & instructions = instance->get_all_instructions();
    if (instructions.empty()) {
        rom_text->append(QStringLiteral("<i>ROM is empty</i>"));
    } else {
        for (int i = 0; i < static_cast<int>(instructions.size()); ++i) {
            std::ostringstream oss;
            if (instructions[i]) oss << *instructions[i];
            QString s = QString::fromStdString(oss.str()).trimmed();
            rom_text->append(QString("%1: %2").arg(i).arg(s));
        }
    }
    int new_idx = indexOf(rom_text);
    if (new_idx != -1) setCurrentIndex(new_idx);
}

void details_tabs::show_compile_errors(std::vector<core::parser::parser_error> const &psr_e)
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
