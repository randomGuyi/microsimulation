//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_SNIPPET_PICKER_H
#define MICROSIMULATION_SNIPPET_PICKER_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class snippet_picker;
}

QT_END_NAMESPACE

class snippet_picker : public QWidget {
    Q_OBJECT

public:
    explicit snippet_picker(QWidget *parent = nullptr);

    ~snippet_picker() override;

private:
    Ui::snippet_picker *ui;
};


#endif //MICROSIMULATION_SNIPPET_PICKER_H