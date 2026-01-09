/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#ifndef MICROSIMULATION_COMPONENT_PICKER_H
#define MICROSIMULATION_COMPONENT_PICKER_H

#include <QWidget>
#include <ui_component_picker.h>
#include <QtWidgets/QPushButton>


namespace gui {
    class drop_target;
}

class drop_target;
QT_BEGIN_NAMESPACE

namespace Ui {class component_picker;}

QT_END_NAMESPACE
namespace gui::views {
    class component_picker : public QWidget {
        Q_OBJECT

    public:
        explicit component_picker(QWidget *parent = nullptr);
        ~component_picker() override;
        void place_all_components(QList<gui::drop_target *> const  & targets);

        QPushButton * get_auto_place_btn(){
            return ui->pushButton_auto_place;
        }

    private:
        Ui::component_picker *ui;
    };
}

// Keep generated UI headers compatible by providing an unqualified alias
using component_picker = gui::views::component_picker;

#endif //MICROSIMULATION_COMPONENT_PICKER_H