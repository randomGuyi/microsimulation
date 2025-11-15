//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_SVG_LOADER_H
#define MICROSIMULATION_SVG_LOADER_H
#include <string>
#include <QtCore/qstring.h>
#include <QtGui/qpixmap.h>
#include <QtSvg/qsvgrenderer.h>

namespace shared {
    class svg_loader {
        public:
        static svg_loader & get_instance() {
            static svg_loader * instance = new svg_loader{};
            return *instance;
        }

        void load_from_file(QString const &file_path);
        QPixmap get_pixmap_for_id(QString const & id, QSize const & size) const;
        QSvgRenderer & get_renderer() const;
        static QRect get_bounds_for_id(const QString &);

    private:
        svg_loader();
        ~svg_loader() = default;

        svg_loader(svg_loader const&) = delete;
        svg_loader& operator=(svg_loader const&) = delete;

        QString m_file_path;
        QSvgRenderer * m_renderer;
    };

    };
} // core

#endif //MICROSIMULATION_SVG_LOADER_H