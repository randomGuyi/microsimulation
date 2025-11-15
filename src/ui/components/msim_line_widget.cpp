#include "msim_line_widget.h"
#include "qgraphicsproxywidget.h"
#include "qgraphicsscene.h"



msim_line_widget::msim_line_widget(msim_line * line,
                                   shared::svg_loader * loader,
                    QString const & element_id,
                    QGraphicsItem * parent)
    : QGraphicsSvgItem{parent}
    , m_line{line}
    , m_element_id{element_id}
{
    m_renderer = &loader->get_renderer();
    setSharedRenderer(m_renderer);
    setElementId(m_element_id);

    // position the line correctly from SVG
    setPos(m_renderer->boundsOnElement(m_element_id).topLeft());

    // purely visual → don’t block events
    setAcceptedMouseButtons(Qt::NoButton);
    setAcceptHoverEvents(false);

    // draw behind everything
    setZValue(-1);
}

QGraphicsProxyWidget * msim_line_widget::attach_to_target(drop_target * target){
    setParent(target);
    target->scene()->addItem(this);
    return nullptr;
}

void msim_line_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_line_widget::show_tooltip(){

}

void msim_line_widget::hide_tooltip(){

}

QString msim_line_widget::id(){
    if(! m_line){
        qWarning() << "m_line in component_widget not set !";
        return QString{};
    }
    return QString::fromStdString(m_line->id());
}

QString msim_line_widget::label(){
    if(! m_line){
        qWarning() << "m_line in component_wlabelget not set !";
        return QString{};
    }
    return QString::fromStdString(m_line->label());
}



