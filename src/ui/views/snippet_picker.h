/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */


#ifndef MICROSIMULATION_SNIPPET_PICKER_H
#define MICROSIMULATION_SNIPPET_PICKER_H

#include <QWidget>

#include "core/microcode/inst_word.h"


QT_BEGIN_NAMESPACE

namespace Ui {class snippet_picker;}

QT_END_NAMESPACE

namespace gui::views {
    class snippet_picker : public QWidget {
        Q_OBJECT

    public:
        explicit snippet_picker(QWidget *parent = nullptr);

        void on_cn_clicked(bool checked);

        void on_condition_clicked(bool checked);

        void on_const_clicked(bool checked);

        void on_load_button_clicked();

        void set_ram_options(core::inst_word &word);

        void set_execute_options(core::inst_word &word);

        void set_ar_options(core::inst_word &word);

        void set_decode_options(core::inst_word &word);

        void set_fetch_options(core::inst_word &word);

        ~snippet_picker() override;

    private:
        Ui::snippet_picker *ui;
    };
}
using snippet_picker = gui::views::snippet_picker;
#endif //MICROSIMULATION_SNIPPET_PICKER_H