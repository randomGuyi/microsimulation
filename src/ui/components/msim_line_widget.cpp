#include "msim_line_widget.h"

#include <qgraphicseffect.h>

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

    setPos(m_renderer->boundsOnElement(m_element_id).topLeft());

    setAcceptedMouseButtons(Qt::NoButton);
    setAcceptHoverEvents(false);

    // draw behind everything
    setZValue(-1);
    m_original_scale = scale();
}

QGraphicsProxyWidget * msim_line_widget::attach_to_target(drop_target * target){
    if (! target) {
        qWarning() << "attack_to_target called with null target!";
        return nullptr;
    }
    if (! target->scene()) {
        qWarning() << "attack_to_target called with target that has no scene!";
        return nullptr;
    }
    setParent(target);
    target->scene()->addItem(this);
    return nullptr;
}

void msim_line_widget::activate() {
    // create and attach a colorize effect to brighten the SVG element
    if (m_highlight_effect == nullptr) {
        auto * effect = new QGraphicsColorizeEffect(this);
        effect->setColor(Qt::red);      // bright tint
        effect->setStrength(1.0);          // adjust to taste
        m_highlight_effect = effect;
        setGraphicsEffect(m_highlight_effect);
    }
    update();
}
void msim_line_widget::deactivate() {
    // remove and delete the effect, restore scale
    if (m_highlight_effect) {
        delete m_highlight_effect;
        setGraphicsEffect(nullptr);
        m_highlight_effect = nullptr;
    }
    update();
}

void msim_line_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_line_widget::show_tooltip(){

}

void msim_line_widget::hide_tooltip(){

}

QString msim_line_widget::id() const{
    if(! m_line){
        qWarning() << "m_line in component_widget not set !";
        return QString{};
    }
    return QString::fromStdString(m_line->id());
}

QString msim_line_widget::label() const{
    if(! m_line){
        qWarning() << "m_line in component_wlabelget not set !";
        return QString{};
    }
    return QString::fromStdString(m_line->label());
}



