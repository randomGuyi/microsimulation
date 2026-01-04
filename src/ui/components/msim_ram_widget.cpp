#include "msim_ram_widget.h"

#include <QLabel>
#include <QMouseEvent>

#include "shared/architecture_ids.h"
#include "ui/controller/details_controller.h"


using namespace gui::components;
using gui::views::details_controller;
msim_ram_widget::msim_ram_widget(core::components::msim_ram * ram,
                                 shared::svg_loader * loader,
                                 QString const & element_id,
                                 QWidget * parent)
        : msim_component_svg_widget{loader, element_id, ram, parent}
, m_ram{ram}
{

    /* get element bounds and RAM-display sub-rect */
    QRectF elem_bounds = m_loader->get_renderer().boundsOnElement(element_id);
    QRectF ram_disp_rect = m_loader->get_renderer().boundsOnElement(ID_DISP_RAM);

    /* translate to widget-local coordinates */
    ram_disp_rect.translate(-ram_disp_rect.topLeft()
                            + QPointF{ram_disp_rect.x() - elem_bounds.x()+ 14.0, 11.0});

    /* compute cell sizes for the 3x3 grid */
    int cols = 3, rows = 3;
    qreal cell_w = ram_disp_rect.width() / cols;
    qreal cell_h = ram_disp_rect.height() / rows;

    /* value font: begrenzt für max. 3 Ziffern */
    QFont valueFont;
    valueFont.setPointSizeF(std::clamp((cell_h * 0.18) / 3, (8.0 / 3), (16.0 / 3)));

    QFont addrFont;
    addrFont.setPointSizeF(std::max<qreal>(2.0, cell_h * 0.1));

    m_value_labels.reserve(rows * cols);
    m_addr_labels.reserve(rows * cols);

    int idx = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {

            QRectF cell_rect(
                ram_disp_rect.left() + c * cell_w,
                ram_disp_rect.top()  + r * cell_h,
                cell_w,
                cell_h
            );

            /* cell background + grid lines */
            auto *bg = new QLabel(this);
            bg->setGeometry(cell_rect.toRect());

            QString style{"background: transparent;"};
            if (c < cols - 1) {
                style += "border-right: 1px solid rgba(0,0,0,0.25);";
            }

            bg->setStyleSheet(style);

            /* cell-internal layout: 70% value / 30% address */
            int val_h  = int(bg->height() * 0.70);
            int addr_h = bg->height() - val_h;

            /* value label */
            auto *val = new QLabel(bg);
            val->setAlignment(Qt::AlignCenter);
            val->setFont(valueFont);
            val->setStyleSheet("color: black; background: transparent;");
            val->setGeometry(0, 0, bg->width(), val_h);

            /* address label */
            auto *addr = new QLabel(bg);
            addr->setFont(addrFont);
            addr->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
            if (r < rows - 1) {
                addr->setStyleSheet("color: rgba(0,0,0,0.6); background: transparent; border-bottom: 1px solid rgba(0,0,0,0.25);");
            }else {
                addr->setStyleSheet("color: rgba(0,0,0,0.6); background: transparent;");
            }
            addr->setGeometry(0, val_h, bg->width(), addr_h);

            /* store pointers */
            m_value_labels.push_back(val);
            m_addr_labels.push_back(addr);

            /* dummy content */
            val->setText("NAN");
            addr->setText(QString::number(idx));

            ++idx;
        }
    }

    /* setup RAM subscription */
    m_ram->subscibe([this](int address_accessed){
        QMetaObject::invokeMethod(this, [this, address_accessed]{
            on_core_value_changed(address_accessed);
        });
    }) ;

    // connect clicks to the global details controller (controller may not have a view yet)
    connect(this, &msim_ram_widget::clicked, &details_controller::instance(), &details_controller::on_ram_clicked);
}

void msim_ram_widget::mousePressEvent(QMouseEvent *event){
    // Emit clicked signal so other parts of the app can show details
    emit clicked(m_ram);
    // Forward to base class to preserve normal button behaviour
    msim_component_svg_widget::mousePressEvent(event);
}

void msim_ram_widget::on_core_value_changed(int address_accessed) {
    /* address, value */
    std::map<int, int> ram_data = m_ram->get_ram_data();
    /* assume the ram has rows with 32 columns, so n * 32 */
    /* the address accessed is always in the center, start is top left*/
    int start_address = address_accessed - (32 + 2);
    for (int i = 0; i < m_addr_labels.size(); ++i) {
        if (i > 0 && i % 3 == 0) {
            start_address += 30; /* next row */
        }
        int current_address = start_address + i;
        m_addr_labels[i]->setText(QString::number(current_address));
        if (current_address < 0) {
            m_value_labels[i]->setText("N/A");
        }
        else if (ram_data.find(current_address) != ram_data.end()) {
            int val = ram_data.at(current_address);
            m_value_labels[i]->setText(QString::number(val));
        } else {
            m_value_labels[i]->setText("N/A");
        }
    }
    update();
}


void msim_ram_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_ram_widget::show_tooltip(){

}

void msim_ram_widget::hide_tooltip(){

}
