#ifndef MSIM_LINE_WIDGET_H
#define MSIM_LINE_WIDGET_H

#include "../../shared/svg_loader.h"
#include "../../core/components/msim_line.h"
#include "../components/msim_component_svg_widget.h"
#include <QGraphicsSvgItem>

class msim_line_widget  :public QGraphicsSvgItem
                        ,public msim_component_view
{
    Q_OBJECT
public:
    explicit msim_line_widget(
        msim_line * line,
        shared::svg_loader * loader,
        QString const & element_id,
         QGraphicsItem * parent = nullptr);

    virtual QGraphicsProxyWidget * attach_to_target(drop_target * target) override;

    virtual void update_display() override;
    virtual void show_tooltip() override;
    virtual void hide_tooltip() override;

    virtual QString id() const override;
    virtual QString label()const override;

private:
    msim_line * m_line;
    QString m_element_id;
    QSvgRenderer * m_renderer;
};

#endif // MSIM_LINE_WIDGET_H
