//
// Created by Arthur on 15.11.25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_snippet_picker.h" resolved

#include "snippet_picker.h"

#include <QTextEdit>

#include "ui_snippet_picker.h"
#include "core/components/msim_cpu.h"
#include "core/components/msim_rom.h"
#include "core/parser/msim_parser.h"
#include "ui/controller/msim_components_manager.h"

using namespace gui::views;
snippet_picker::snippet_picker(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::snippet_picker)
{
    ui->setupUi(this);
    QTextEdit *edit = new QTextEdit(this);
    edit->setPlainText("// Snippet Picker\n// Select a code snippet from the list below:\n\n");
    connect(ui->load_to_rom_btn, &QPushButton::clicked, this, [this] {
        on_load_button_clicked();
    });

    connect(ui->radioButton_constant, &QRadioButton::toggled, this, [this](bool checked){
        on_const_clicked(checked);
    });

    connect (ui->radioButton_4cop_cond, &QRadioButton::toggled, this, [this](bool checked) {
        on_condition_clicked(checked);
    });

    connect (ui->radioButton_car_cn, &QRadioButton::toggled, this, [this](bool checked) {
        on_cn_clicked(checked);
    });
}

void snippet_picker::on_cn_clicked(bool checked) {
    if (!checked) {return;}
    ui->radioButton_carplspls->setChecked(false);
    ui->radioButton_4cop->setChecked(false);
    ui->radioButton_4cop_cond->setChecked(false);
}

void snippet_picker::on_condition_clicked(bool checked) {
    if (!checked) {return;}
    ui->radioButton_carplspls->setChecked(false);
    ui->radioButton_car_cn->setChecked(false);
    ui->radioButton_4cop->setChecked(false);
}

void snippet_picker::on_const_clicked(bool checked) {
    if (!checked) {return;}
    ui->radioButton_z_z->setChecked(false);
    ui->radioButton_z_x->setChecked(false);
    ui->radioButton_z_y->setChecked(false);
    ui->radioButton_z_xplsy->setChecked(false);
    ui->radioButton_z_xminy->setChecked(false);
    ui->radioButton_z_xshly->setChecked(false);
    ui->radioButton_z_inc_x->setChecked(false);
    ui->radioButton_z_inc_y->setChecked(false);
    ui->radioButton_z_dec_x->setChecked(false);
    ui->radioButton_z_dec_y->setChecked(false);
    ui->radioButton_z_minx->setChecked(false);
    ui->radioButton_z_xtimesy->setChecked(false);
    ui->radioButton_z_xdivy->setChecked(false);
}


void snippet_picker::on_load_button_clicked() {
    auto * rom = fac::msim_components_manager::get_instance().get_rom();
    if (! rom) {
        qDebug() << "rom not found, implement an mechanism...";
        return;
    }
    core::inst_word instruction{};
    /* segment */
    int line = ui->spinBox_line->value();
    if (line % 4 == 0) {
        int segment = ui->spinBox_segment->value();
        if (! core::parser::msim_parser::check_segment(segment, line)) {
            qDebug() << "line does not match segment, implement an error mechanism...";
            return;
        };
    }

    set_fetch_options(instruction);
    set_decode_options(instruction);
    set_ar_options(instruction);
    set_execute_options(instruction);
    set_ram_options(instruction);

    rom->add_inst(std::move(instruction), line);
}

void snippet_picker::set_ram_options(core::inst_word & word) {
    if (ui->radioButton_ram_wait->isChecked()) {
        word.set_ram_mode(ram_mode::WAIT);
    }else if (ui->radioButton_ram_read->isChecked()) {
        word.set_ram_mode(ram_mode::READ);
    }else if (ui->radioButton_ram_write->isChecked()) {
        word.set_ram_mode(ram_mode::WRITE);
    }
}

void snippet_picker::set_execute_options(core::inst_word & word) {
    if (ui->checkBox_z_r0->isChecked()) {word.set_z_selection(REGISTER_0);}
    if (ui->checkBox_z_r1->isChecked()) {word.set_z_selection(REGISTER_1);}
    if (ui->checkBox_z_r2->isChecked()) {word.set_z_selection(REGISTER_2);}
    if (ui->checkBox_z_r3->isChecked()) {word.set_z_selection(REGISTER_3);}
    word.set_z_mar(ui->checkBox_z_mar->isChecked());
    word.set_z_mdr(ui->checkBox_z_mdr->isChecked());
}

void snippet_picker::set_ar_options(core::inst_word & word) {
    if (ui->radioButton_carplspls->isChecked()) {
        word.set_ar_mode(ar_mode::CHAR_PLS_PLS);
    }else if (ui->radioButton_car_cn->isChecked()) {
        int value = ui->spinBox_cn->value();
        word.set_cn(value);
        word.set_ar_mode(ar_mode::_4CN);
    }else if (ui->radioButton_4cop->isChecked()) {
        word.set_ar_mode(ar_mode::_4COP);
    }else if (ui->radioButton_4cop_cond->isChecked()) {
        /* Z + - Of */            /*z+-Of*/
        uint8_t mask = 0; /* 0000 0000*/
        if (ui->checkBox_z_cond->isChecked()) {mask |=   0b00001000;}
        if (ui->checkBox_pos_cond->isChecked()) {mask |= 0b00000100;}
        if (ui->checkBox_neg_cond->isChecked()) {mask |= 0b00000010;}
        if (ui->checkBox_of_cond->isChecked()) {mask |=  0b00000001;}
        word.set_mask(mask);
        word.set_ar_mode(ar_mode::_4COP_CND);
    }
}

void snippet_picker::set_decode_options(core::inst_word & word) {
    if (ui->radioButton_constant->isChecked()) {
        int value = ui->spinBox_constant->value();
        word.set_constant_nbr(value);
        word.set_operation(Z_CONST);
    }else if (ui->radioButton_z_z->isChecked()) {
        word.set_default_operation();
    }else if (ui->radioButton_z_x->isChecked()) {
        word.set_operation(Z_X);
    }else if (ui->radioButton_z_y->isChecked()) {
        word.set_operation(Z_Y);
    }else if (ui->radioButton_z_xplsy->isChecked()) {
        word.set_operation(Z_X_PLS_Y);
    }else if (ui->radioButton_z_xminy->isChecked()) {
        word.set_operation(Z_X_MINUS_Y);
    }else if (ui->radioButton_z_xshly->isChecked()) {
        word.set_operation(Z_X_SHL_Y);
    }else if (ui->radioButton_z_inc_x->isChecked()) {
        word.set_operation(Z_INC_X);
    }else if (ui->radioButton_z_inc_y->isChecked()) {
        word.set_operation(Z_INC_Y);
    }else if (ui->radioButton_z_dec_x->isChecked()) {
        word.set_operation(Z_DEC_X);
    }else if (ui->radioButton_z_dec_y->isChecked()) {
        word.set_operation(Z_DEC_Y);
    }else if (ui->radioButton_z_minx->isChecked()) {
        word.set_operation(Z_MINUS_X);
    }else if (ui->radioButton_z_xtimesy->isChecked()) {
        word.set_operation(Z_X_TIMES_Y);
    }else if (ui->radioButton_z_xdivy->isChecked()) {
        word.set_operation(Z_X_DIV_Y);
    }
}

void snippet_picker::set_fetch_options(core::inst_word & word) {
    /* X-Register */
    if (ui->radioButton_r0_x->isChecked()) {
        word.set_x_selection(REGISTER_0);
    } else if (ui->radioButton_r1_x->isChecked()) {
        word.set_x_selection(REGISTER_1);
    } else if (ui->radioButton_r2_x->isChecked()) {
        word.set_x_selection(REGISTER_2);
    } else if (ui->radioButton_r3_x->isChecked()) {
        word.set_x_selection(REGISTER_3);
    }
    /* Y-Register */
    if (ui->radioButton_r0_y->isChecked()) {
        word.set_y_selection(REGISTER_0);
    } else if (ui->radioButton_r1_y->isChecked()) {
        word.set_y_selection(REGISTER_1);
    } else if (ui->radioButton_r2_y->isChecked()) {
        word.set_y_selection(REGISTER_2);
    } else if (ui->radioButton_r3_y->isChecked()) {
        word.set_y_selection(REGISTER_3);
    }
    /* mdr */
    if (ui->checkBox_mdr_cop->isChecked())
        word.set_mdr_cop(true);
    if (ui->checkBox_mdr_y->isChecked())
        word.set_mdr_y(true);
}

snippet_picker::~snippet_picker()
{
    delete ui;
}