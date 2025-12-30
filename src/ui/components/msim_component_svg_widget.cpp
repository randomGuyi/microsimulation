#include "msim_component_svg_widget.h"
#include "qgraphicsproxywidget.h"
#include "qgraphicsscene.h"
#include "qpainter.h"
#include "ui/components/debug_proxy.h"

using namespace gui::components;
msim_component_svg_widget::msim_component_svg_widget(
                                         shared::svg_loader * loader,
                                         QString const & element_id,
                                         core::components::msim_component * comp,
                                         QWidget * parent )
    : msim_component_widget{comp, parent}
    , m_loader{loader}
    , m_element_id{element_id}
    , m_renderer{loader ? &loader->get_renderer() : nullptr}
{
    setFlat(true);
    setStyleSheet("background: transparent; border: none;");
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    if (!m_renderer) {
       qDebug() << "Renderer not found for element:" << element_id;
    }
}
QSize msim_component_svg_widget::sizeHint() const{
    if(m_renderer && m_renderer->isValid()){
        return m_renderer->boundsOnElement(m_element_id).size().toSize();
    }
    return QPushButton::sizeHint();

}

QGraphicsProxyWidget * msim_component_svg_widget::attach_to_target(gui::drop_target * target){

    QGraphicsProxyWidget * proxy =  target->scene()->addWidget(this);
    QRectF bounds = m_loader->get_renderer().boundsOnElement(id());
    this->setFixedSize(bounds.size().toSize()); /* --> was a problem */
    proxy->setPos(bounds.topLeft());
    proxy->setZValue(1);

    proxy->setAcceptedMouseButtons(Qt::AllButtons);
    proxy->setAcceptHoverEvents(true);
    proxy->setAcceptDrops(false);
    return proxy;

}


void msim_component_svg_widget::paintEvent(QPaintEvent * event) {
    msim_component_widget::paintEvent(event);

    if (m_renderer && m_renderer->isValid()) {
        QPainter painter(this);
        m_renderer->render(&painter, m_element_id);
    }
}
