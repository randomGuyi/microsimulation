//
// Created by Arthur on 15.11.25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_simulator_area.h" resolved

#include "simulator_area.h"
#include "ui_simulator_area.h"
#include "ui/drag_and_drop/drop_target.h"
#include "ui_simulator_area.h"
#include "QGraphicsItem"
#include "QGraphicsSvgItem"
#include "QSvgRenderer"
#include "QWheelEvent"
#include "QSvgRenderer"
#include <QStringLiteral>
#include <QFile>
#include "core/common/id_reader.h"
#include <shared/svg_loader.h>
#include <ui/controller/msim_components_manager.h>

simulator_area::simulator_area(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::simulator_area)
    , m_scene{new QGraphicsScene(this)}
    , m_background{nullptr}
    , m_renderer{&shared::svg_loader::get_instance().get_renderer()}
    , m_zoom_factor{1.0}
    , m_auto_fit_enabled{true}
{
    ui->setupUi(this);
    /* read the targets from the id's - file */
    QStringList targets {id_reader::get_instance().get_results_for("^dropTarget_.*")};

    for (const QString &id : targets) {
        if (m_renderer->elementExists(id)) {
                auto * target_item = new drop_target{id};
                m_scene->addItem(target_item);
                m_drop_targets.append(target_item);
        }
    }

    // Assign scene to QGraphicsView
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

QList<drop_target *> & simulator_area::get_drop_targets(){
    return m_drop_targets;
}

void simulator_area::resizeEvent(QResizeEvent * event){
    QWidget::resizeEvent(event);
    if(m_auto_fit_enabled && m_background){
        ui->graphicsView->fitInView(m_background, Qt::KeepAspectRatio);
        m_zoom_factor = 1.0;
    }
}

void simulator_area::wheelEvent(QWheelEvent * event){
    if(event->modifiers() & Qt::ControlModifier){ // zoom only if Ctrl is held
        m_auto_fit_enabled = false;

        const double zoom_step {1.15};
        if(event->angleDelta().y() > 0){
            m_zoom_factor *= zoom_step;
            ui->graphicsView->scale(zoom_step, zoom_step);
        }else{
            m_zoom_factor /= zoom_step;
            ui->graphicsView->scale(1.0/zoom_step, 1.0/zoom_step);
        }
    }else{
        QWidget::wheelEvent(event);
    }
}

void simulator_area::resetZoom(){
    m_auto_fit_enabled = true;
    resizeEvent(nullptr); // refit
}


simulator_area::~simulator_area(){
    delete ui;
}