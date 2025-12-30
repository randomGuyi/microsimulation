//
// Created by Arthur on 15.11.25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_component_picker.h" resolved

#include "component_picker.h"
#include "ui_component_picker.h"


#include <QStandardItemModel>
#include <QSvgRenderer>
#include <QPainter>

#include "ui/drag_and_drop/drag_item_model.h"
#include "ui_component_picker.h"
#include "ui/controller/components_picker_controller.h"

#include <ui/drag_and_drop/drop_target.h>

using namespace gui::views;

component_picker::component_picker(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::component_picker)
{
    ui->setupUi(this);
    drag_item_model * model = new drag_item_model{this};
    ui->listView->setModel(model);
    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listView->setDragEnabled(true);
    ui->listView->setAcceptDrops(false);
    ui->listView->setDropIndicatorShown(false);
    ui->listView->setDefaultDropAction(Qt::MoveAction);
    ui->listView->setDragDropMode(QAbstractItemView::DragOnly);

    components_picker_controller controller{};
    auto components = controller.get_all_components();

    for(auto & comp : components){
        QStandardItem * item{new QStandardItem};
        item->setTextAlignment(Qt::AlignCenter);

        item->setData(comp.id, Qt::UserRole);
        item->setData(comp.label , Qt::DisplayRole);

        item->setEditable(false);
        item->setDragEnabled(true);

        if(! comp.icon.isNull()){
            /* its not a register */
            item->setIcon(QIcon(comp.icon));
        }

        model->appendRow(item);
    }
}

void component_picker::place_all_components(QList<drop_target *> const & targets){
    QAbstractItemModel * model = ui->listView->model();

    for(auto & tgt_obj : targets){
        drop_target * tgt_ptr = dynamic_cast<drop_target *>(tgt_obj);
        if(! tgt_ptr){
            throw std::runtime_error("[component_picker] can only place component of object of 'drop_target'");
        }

        for(int row = 0; row < model->rowCount(); ++row){
            QModelIndex index = model->index(row, 0);

            QString const id = model->data(index, Qt::UserRole).toString();
            QString const label = model->data(index, Qt::DisplayRole).toString();


            /* find right element */
            if(tgt_ptr->elementId().remove("dropTarget_") ==  QString{id}.remove("comp_")){
                /* place element */
                tgt_ptr->place_component(id, label);
                /* delete the element from the component picker*/
                model->removeRow(row);
                /* look for the next target */
                break;
            }
        }
    }
}

component_picker::~component_picker()
{
    delete ui;
}
