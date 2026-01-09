/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MSIM_AR_WIDGET_H
#define MSIM_AR_WIDGET_H

#include "msim_component_svg_widget.h"

#include "../../shared/svg_loader.h"
#include "../../core/components/msim_ar.h"


namespace gui::components {
    class msim_ar_widget : public msim_component_svg_widget{
        Q_OBJECT
    public:
        msim_ar_widget(core::components::msim_ar * ar,
                       shared::svg_loader * loader,
                       QString const & element_id,
                       QWidget * parent = nullptr);

        void update_display() override;

        void show_tooltip() override;
        void hide_tooltip() override;
    protected:
        core::components::msim_ar * m_ar{};


    };
}

#endif // MSIM_AR_WIDGET_H
