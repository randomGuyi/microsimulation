#ifndef MSIM_COMPONENT_WIDGET_H
#define MSIM_COMPONENT_WIDGET_H

#include <QPushButton>

#include "core/components/msim_component.h"
#include "msim_component_view.h"
#include "ui/drag_and_drop/drop_target.h"

class msim_component_widget : public QPushButton,
                              public msim_component_view{
    Q_OBJECT

public:
    msim_component_widget(msim_component * comp, QWidget * parent = nullptr);

    virtual QGraphicsProxyWidget * attach_to_target(drop_target * target) override;

    virtual QString id() override;
    virtual QString label() override;

protected:
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

    msim_component * m_component;
};

#endif // MSIM_COMPONENT_WIDGET_H
