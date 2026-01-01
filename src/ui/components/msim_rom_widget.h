#ifndef MSIM_ROM_WIDGET_H
#define MSIM_ROM_WIDGET_H

#include <QLabel>

#include "msim_component_svg_widget.h"

#include <shared/svg_loader.h>
#include <core/components/msim_rom.h>

#include "ui/highlight_label.h"

namespace gui::components {
    class msim_rom_widget : public msim_component_svg_widget{
        Q_OBJECT
    public:
        msim_rom_widget(core::components::msim_rom * rom,
                        shared::svg_loader * loader,
                        QString const & element_id,
                        QWidget * parent = nullptr);


        virtual void update_display() override;

        virtual void show_tooltip() override;
        virtual void hide_tooltip() override;
    protected:
        core::components::msim_rom * m_rom;
        std::vector<HighlightLabel> m_instruction_lines;
        int m_old_line = 0;
    private:

        void on_core_value_changed(int new_val);

    signals:
        void clicked(core::components::msim_rom * rom);

    protected:
        void mousePressEvent(QMouseEvent * event) override;
    };
}
#endif // MSIM_ROM_WIDGET_H
