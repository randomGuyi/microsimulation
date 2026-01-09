/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#include "msim_component_widget.h"
#include "qgraphicsscene.h"

#include <QGraphicsProxyWidget>

#include <shared/svg_loader.h>
using namespace gui::components;
msim_component_widget::msim_component_widget(core::components::msim_component * comp, QWidget * parent)
: QPushButton{parent}
, m_component{comp}
{}

void msim_component_widget::enterEvent(QEnterEvent* event){
    Q_UNUSED(event);
    this->show_tooltip();
}

void msim_component_widget::leaveEvent(QEvent* event){
    Q_UNUSED(event);
    this->hide_tooltip();
}

QGraphicsProxyWidget *msim_component_widget::attach_to_target(drop_target * target){
    QGraphicsProxyWidget * proxy = target->scene()->addWidget(this);
    proxy->setParentItem(target);
    proxy->setGeometry(target->boundingRect());
    proxy->setZValue(1);

    proxy->setAcceptedMouseButtons(Qt::AllButtons);
    proxy->setAcceptHoverEvents(true);
    proxy->setAcceptDrops(false);
    return proxy;
}

QString msim_component_widget::id() const {
    if(! m_component){
        qWarning() << "m_component in component_widget not set !";
        return QString{};
    }
    return QString::fromStdString(m_component->id());
}

QString msim_component_widget::label() const {
    if(! m_component){
        qWarning() << "m_component in component_widget not set !";
        return QString{};
    }
    return QString::fromStdString(m_component->label());
}
