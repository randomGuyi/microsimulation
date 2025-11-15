//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_SIMULATOR_AREA_H
#define MICROSIMULATION_SIMULATOR_AREA_H

#include <QGraphicsScene>
#include <QSvgRenderer>
#include <QWidget>
#include <QGraphicsSvgItem>


class drop_target;
QT_BEGIN_NAMESPACE

namespace Ui {
    class simulator_area;
}

QT_END_NAMESPACE

class simulator_area : public QWidget {
    Q_OBJECT

public:
    explicit simulator_area(QWidget *parent = nullptr);
    ~simulator_area();
    QList<drop_target *> & get_drop_targets();


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


#endif //MICROSIMULATION_SIMULATOR_AREA_H