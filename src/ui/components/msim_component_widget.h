#ifndef MSIM_COMPONENT_WIDGET_H
#define MSIM_COMPONENT_WIDGET_H

#include <QPushButton>

#include "core/components/msim_component.h"
#include "msim_component_view.h"
#include "ui/drag_and_drop/drop_target.h"

namespace gui::components {
    class msim_component_widget : public QPushButton,
                                  public msim_component_view{
        Q_OBJECT

    public:
        msim_component_widget(core::components::msim_component * comp, QWidget * parent = nullptr);

        virtual QGraphicsProxyWidget * attach_to_target(drop_target * target) override;


        virtual QString id() const override;
        virtual QString label() const override;

    protected:
        virtual void enterEvent(QEnterEvent* event) override;
        virtual void leaveEvent(QEvent* event) override;

        core::components::msim_component * m_component;
    };
}
#endif // MSIM_COMPONENT_WIDGET_H
