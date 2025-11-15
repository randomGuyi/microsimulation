//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_COMPONENT_PICKER_H
#define MICROSIMULATION_COMPONENT_PICKER_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class component_picker;
}

QT_END_NAMESPACE

class component_picker : public QWidget {
    Q_OBJECT

public:
    explicit component_picker(QWidget *parent = nullptr);
    ~component_picker() override;
    void place_all_components(QList<drop_target *> const  & targets);

private:
    Ui::component_picker *ui;
};


#endif //MICROSIMULATION_COMPONENT_PICKER_H