//
// Created by Arthur on 19.11.25.
//

#include "msim_decoder_widget.h"

#include <QGraphicsScene>

msim_decoder_widget::msim_decoder_widget(
    msim_decoder *decoder,
    shared::svg_loader *loader,
    QString const &element_id,
    QGraphicsSvgItem *parent)
: QGraphicsSvgItem{parent}
, m_decoder{decoder}
, m_element_id{element_id} {

    m_renderer = &loader->get_renderer();
    setSharedRenderer(m_renderer);
    setElementId(m_element_id);

    // position the line correctly from SVG
    setPos(m_renderer->boundsOnElement(m_element_id).topLeft());

    // purely visual → don’t block events
    setAcceptedMouseButtons(Qt::NoButton);
    setAcceptHoverEvents(false);

    // draw behind everything
   // setZValue(-1);
}

QGraphicsProxyWidget * msim_decoder_widget::attach_to_target(drop_target * target){
    setParent(target);
    target->scene()->addItem(this);
    return nullptr;
}

void msim_decoder_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_decoder_widget::show_tooltip(){

}

void msim_decoder_widget::hide_tooltip(){

}

QString msim_decoder_widget::id() const{
    if(! m_decoder){
        qWarning() << "m_decoder in component_widget not set !";
        return QString{};
    }
    return QString::fromStdString(m_decoder->id());
}

QString msim_decoder_widget::label() const{
    if(! m_decoder){
        qWarning() << "m_decoder in component_widget not set !";
        return QString{};
    }
    return QString::fromStdString(m_decoder->label());
}



