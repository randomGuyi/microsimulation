#ifndef MSIM_RAM_WIDGET_H
#define MSIM_RAM_WIDGET_H

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

    virtual void update_display() override;

    virtual void show_tooltip() override;
    virtual void hide_tooltip() override;
protected:
    msim_ram * m_ram;

};

#endif // MSIM_RAM_WIDGET_H
