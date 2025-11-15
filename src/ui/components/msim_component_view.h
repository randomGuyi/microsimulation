#ifndef MSIM_COMPONENT_VIEW_H
#define MSIM_COMPONENT_VIEW_H

#include "ui/drag_and_drop/drop_target.h"
#include <QGraphicsProxyWidget>
#include <QString>


class msim_component_view{
public:
    virtual ~msim_component_view() = default;

    virtual QGraphicsProxyWidget * attach_to_target(drop_target * target) = 0;

    virtual void update_display() = 0;

    virtual void show_tooltip() = 0;
    virtual void hide_tooltip() = 0;

    virtual QString id() = 0;
    virtual QString label() = 0;

//    virtual QGraphicsItem * graphics_item() = 0;

};

#endif // MSIM_COMPONENT_VIEW_H
