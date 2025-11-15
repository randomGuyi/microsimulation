//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_DRAG_ITEM_MODEL_H
#define MICROSIMULATION_DRAG_ITEM_MODEL_H
#include <QStandardItemModel>

#define CUSTOM_ID_DATA_STR "application/x-custom-id"
#define CUSTOM_LABEL_DATA_STR "application/x-custom-label"
#define PLAIN_TEXT_DATA_STR "text/plain"

class drag_item_model : public QStandardItemModel{
public:
    using QStandardItemModel::QStandardItemModel;

    QMimeData * mimeData(const QModelIndexList & indexes) const override;
    QStringList mimeTypes() const override;
};



#endif //MICROSIMULATION_DRAG_ITEM_MODEL_H