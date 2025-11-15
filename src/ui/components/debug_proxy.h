#ifndef DEBUG_PROXY_H
#define DEBUG_PROXY_H

#include "qpainter.h"
#include <QGraphicsProxyWidget>

class DebugProxy : public QGraphicsProxyWidget {
public:
    using QGraphicsProxyWidget::QGraphicsProxyWidget;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override {
        // Call base class to paint child widget
        QGraphicsProxyWidget::paint(painter, option, widget);

        // Overlay debug color
        painter->setBrush(QColor(255, 0, 0, 50));   // semi-transparent red
        painter->setPen(Qt::DashLine);
        painter->drawRect(boundingRect());
    }
};

#endif // DEBUG_PROXY_H
