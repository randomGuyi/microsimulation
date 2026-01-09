/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */


#ifndef MICROSIMULATION_SIMULATOR_AREA_H
#define MICROSIMULATION_SIMULATOR_AREA_H

#include <QGraphicsScene>
#include <QSvgRenderer>
#include <QWidget>
#include <QGraphicsSvgItem>

#include "ui/drag_and_drop/drop_target.h"


class drop_target;
QT_BEGIN_NAMESPACE

namespace Ui {class simulator_area;}

QT_END_NAMESPACE
namespace gui::views {
    class simulator_area : public QWidget {
        Q_OBJECT

    public:
        explicit simulator_area(QWidget *parent = nullptr);
        ~simulator_area();
        QList<gui::drop_target *> & get_drop_targets();


    protected:
        void resizeEvent(QResizeEvent * event) override;
        void wheelEvent(QWheelEvent * event) override;

    private:
        Ui::simulator_area *ui;
        QGraphicsScene * m_scene;
        QGraphicsSvgItem * m_background;
        QSvgRenderer * m_renderer;
        QList<drop_target *> m_drop_targets;


        double m_zoom_factor;
        bool m_auto_fit_enabled;


    public slots:
        void resetZoom();
    };
}

// Provide unqualified alias for compatibility with uic-generated headers
using simulator_area = gui::views::simulator_area;

#endif //MICROSIMULATION_SIMULATOR_AREA_H