#ifndef MSIM_RAM_WIDGET_H
#define MSIM_RAM_WIDGET_H

#include <QLabel>

#include "msim_component_svg_widget.h"

#include <shared/svg_loader.h>
#include <core/components/msim_ram.h>


class msim_ram_widget : public msim_component_svg_widget{
    Q_OBJECT
public:
    msim_ram_widget(msim_ram * ram,
                    shared::svg_loader * loader,
                    QString const & element_id,
                    QWidget * parent = nullptr);

    void on_core_value_changed(int address_accessed);

    void update_display() override;

    void show_tooltip() override;
    void hide_tooltip() override;
protected:
    msim_ram * m_ram;

    std::vector<QLabel*> m_value_labels;
    std::vector<QLabel*> m_addr_labels;

};

#endif // MSIM_RAM_WIDGET_H
