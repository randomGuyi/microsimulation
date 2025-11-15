#ifndef MSIM_COMPONENT_SVG_WIDGET_H
#define MSIM_COMPONENT_SVG_WIDGET_H

#include "msim_component_widget.h"
#include "../../shared/svg_loader.h"


class drop_target;

class msim_component_svg_widget : public msim_component_widget{
    Q_OBJECT
public:
    explicit msim_component_svg_widget(shared::svg_loader * loader, QString const & element_id, msim_component * comp, QWidget * parent = nullptr);
    virtual QGraphicsProxyWidget * attach_to_target(drop_target * target) override;

protected:
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent * event) override;

    shared::svg_loader * m_loader;
    QString m_element_id;
    QSvgRenderer * m_renderer;
};

#endif // MSIM_COMPONENT_SVG_WIDGET_H
