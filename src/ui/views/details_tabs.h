/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */


#ifndef MICROSIMULATION_DETAILS_TABS_H
#define MICROSIMULATION_DETAILS_TABS_H
#include <QTabWidget>
#include <QTextEdit>

#include "../../core/parser/msim_parser.h"
#include "../../core/components/msim_rom.h"
#include "../../core/components/msim_ram.h"


#define COMPILER_TAB_LABEL "Compiler Output"
#define ROM_TAB_LABEL "ROM Details"
#define RAM_TAB_LABEL "RAM Details"
namespace gui::views {
    class details_tabs : public QTabWidget
    {
        Q_OBJECT
    public:
        details_tabs(QWidget *parent = nullptr);
        void show_ram_details(core::components::msim_ram *instance);
        void show_rom_details(core::components::msim_rom *instance);
        void show_compile_errors(std::vector<core::parser::parser_error> const &psr_e);
        void show_compile_details(std::string const &msg);

    private:
        int tab_index_for_label(std::string const &lbl);
        QTextEdit *get_or_make_compiler_log();
        QTextEdit * m_ram_text_view{nullptr};
    };
}

#endif //MICROSIMULATION_DETAILS_TABS_H

