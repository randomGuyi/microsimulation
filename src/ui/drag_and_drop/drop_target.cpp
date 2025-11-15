//
// Created by Arthur on 15.11.25.
//

#include "drop_target.h"

#include <QGraphicsSceneEvent>
#include <QMimeData>
#include <QToolTip>

#include "ui/drag_and_drop/drag_item_model.h"
#include "ui/controller/msim_components_manager.h"

drop_target::drop_target(const QString & element_id)
    :QGraphicsSvgItem{}
{
    QSvgRenderer * r = &shared::svg_loader::get_instance().get_renderer();
    if(r){
        setElementId(element_id);
        setSharedRenderer(r);

        setPos(r->boundsOnElement(element_id).topLeft());
        setZValue(0);
    }else{
        qDebug() << "no renderer found in drop_target";
    }
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    /* tooltip */
    QFont tooltip_font{};
    tooltip_font.setPointSize(14);
    QToolTip::setFont(tooltip_font);

}


void drop_target::dragEnterEvent (QGraphicsSceneDragDropEvent * event) {
     if (event->mimeData()->hasFormat(CUSTOM_ID_DATA_STR)) {
        QString dropped_id = QString::fromUtf8(event->mimeData()->data(CUSTOM_ID_DATA_STR));
         dropped_id.remove("comp_");

         if(dropped_id == clean_element_id()){
             event->acceptProposedAction();
             setOpacity(0.5);
         }else{
             event->ignore();
         }
     }
     QGraphicsSvgItem::dragEnterEvent(event);
}

void drop_target::dragLeaveEvent (QGraphicsSceneDragDropEvent * event) {
     setOpacity(1.0);
     QGraphicsSvgItem::dragEnterEvent(event);
}


void drop_target::dropEvent (QGraphicsSceneDragDropEvent * event) {
    setOpacity(1.0);

     if (event->mimeData()->hasFormat(CUSTOM_ID_DATA_STR)) {
         QString dropped_id = QString::fromUtf8(event->mimeData()->data(CUSTOM_ID_DATA_STR));
         QString dropped_label = QString::fromUtf8(event->mimeData()->data(CUSTOM_LABEL_DATA_STR));
         place_component(dropped_id, dropped_label);

        event->acceptProposedAction();

     } else {
         event->ignore();
     }
}

void drop_target::place_component(QString const & id, QString const & label){
    msim_components_manager::get_instance().place_component(this, id, label);
}

void drop_target::hoverEnterEvent (QGraphicsSceneHoverEvent * event) {
    QString tooltip = QString{"element id : " + elementId()};
    QToolTip::showText(cursor().pos(), tooltip, nullptr, QRect(), 5000); /* show tooltip for 5 sec */
     QGraphicsSvgItem::hoverEnterEvent(event);
}

void drop_target::hoverLeaveEvent (QGraphicsSceneHoverEvent * event) {
    QToolTip::hideText();
     QGraphicsSvgItem::hoverLeaveEvent(event);
}

drop_target::~drop_target(){
}
QString drop_target::clean_element_id() const{
    return elementId().remove("dropTarget_");
}

