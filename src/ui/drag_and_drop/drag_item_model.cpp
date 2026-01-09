/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#include "drag_item_model.h"

#include <QMimeData>

using namespace gui;

QMimeData * drag_item_model::mimeData(const QModelIndexList & indexes) const {
    if(indexes.isEmpty()){
        return nullptr;
    }
    QMimeData * mime_data{new QMimeData{}};
    const QModelIndex & index = indexes.first();

    QString original_id = index.data(Qt::UserRole).toString();
    QString display_name = index.data(Qt::DisplayRole).toString();

    mime_data->setText(original_id);
    mime_data->setData(CUSTOM_ID_DATA_STR, original_id.toUtf8());
    mime_data->setData(CUSTOM_LABEL_DATA_STR, display_name.toUtf8());

    return mime_data;
}

QStringList drag_item_model::mimeTypes() const {
    return {
        CUSTOM_ID_DATA_STR,
        CUSTOM_LABEL_DATA_STR,
        PLAIN_TEXT_DATA_STR
    };
}
