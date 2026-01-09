/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MSIM_COMPONENT_SVG_WIDGET_H
#define MSIM_COMPONENT_SVG_WIDGET_H

#include "msim_component_widget.h"
#include "../../shared/svg_loader.h"

namespace gui::components {
    class drop_target;

    class msim_component_svg_widget : public msim_component_widget{
        Q_OBJECT
    public:
        explicit msim_component_svg_widget(shared::svg_loader * loader, QString const & element_id,
            core::components::msim_component * comp, QWidget * parent = nullptr);

        QGraphicsProxyWidget * attach_to_target(gui::drop_target * target) override;

    protected:
        QSize sizeHint() const override;
        void paintEvent(QPaintEvent * event) override;

        shared::svg_loader * m_loader;
        QString m_element_id;
        QSvgRenderer * m_renderer;
    };
}
#endif // MSIM_COMPONENT_SVG_WIDGET_H
