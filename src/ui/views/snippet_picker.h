//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_SNIPPET_PICKER_H
#define MICROSIMULATION_SNIPPET_PICKER_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {class snippet_picker;}

QT_END_NAMESPACE

namespace gui::views {
    class snippet_picker : public QWidget {
        Q_OBJECT

    public:
        explicit snippet_picker(QWidget *parent = nullptr);
        ~snippet_picker() override;

    private:
        Ui::snippet_picker *ui;
    };
}
using snippet_picker = gui::views::snippet_picker;
#endif //MICROSIMULATION_SNIPPET_PICKER_H