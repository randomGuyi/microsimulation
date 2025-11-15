#ifndef MSIM_ROM_WIDGET_H
#define MSIM_ROM_WIDGET_H

#include "msim_component_svg_widget.h"

#include <shared/svg_loader.h>
#include <core/components/msim_rom.h>


class msim_rom_widget : public msim_component_svg_widget{
    Q_OBJECT
public:
    msim_rom_widget(msim_rom * rom,
                    shared::svg_loader * loader,
                    QString const & element_id,
                    QWidget * parent = nullptr);

    virtual void update_display() override;

    virtual void show_tooltip() override;
    virtual void hide_tooltip() override;
protected:
    msim_rom * m_rom;

};

#endif // MSIM_ROM_WIDGET_H
