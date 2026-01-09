/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#ifndef MICROSIMULATION_SVG_LOADER_H
#define MICROSIMULATION_SVG_LOADER_H
#include <string>
#include <QtCore/qstring.h>
#include <QtGui/qpixmap.h>
#include <QtSvg/qsvgrenderer.h>

namespace shared {
    class svg_loader {
    public:
        static svg_loader &get_instance() {
            static auto instance = new svg_loader{};
            return *instance;
        }

        void load_from_file(QString const &file_path);
        [[nodiscard]] QPixmap get_pixmap_for_id(QString const &id, QSize const &size) const;
        [[nodiscard]] QSvgRenderer &get_renderer() const;

    private:
        svg_loader();
        ~svg_loader() = default;
        svg_loader(svg_loader const &) = delete;
        svg_loader &operator=(svg_loader const &) = delete;
        QString m_file_path;
        QSvgRenderer *m_renderer;
    };
};

#endif //MICROSIMULATION_SVG_LOADER_H
