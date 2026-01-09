/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#include "svg_loader.h"
#include <QDebug>
#include <QPainter>

#include "constants.h"

using namespace shared;

svg_loader::svg_loader()
    : m_renderer{new QSvgRenderer{}} {
    load_from_file(SVG_PATH);
}

void svg_loader::load_from_file(QString const &file_path) {
    if (!m_renderer->load(file_path)) {
        qWarning() << "SVG path not found or invalid:" << file_path;
        return;
    }
    m_file_path = file_path;
}

QPixmap svg_loader::get_pixmap_for_id(QString const &id, QSize const &size) const {
    if (!m_renderer->isValid() || !m_renderer->elementExists(id)) {
        qWarning() << "SVG element not found:" << id;
        return QPixmap{};
    }

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    m_renderer->render(&painter, id);
    return pixmap;
}

QSvgRenderer &svg_loader::get_renderer() const {
    return *m_renderer;
}
