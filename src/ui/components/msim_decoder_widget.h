//
// Created by Arthur on 19.11.25.
//

#ifndef MICROSIMULATION_MSIM_DECODER_WIDGET_H
#define MICROSIMULATION_MSIM_DECODER_WIDGET_H
#include <QGraphicsSvgItem>

#include "msim_component_view.h"
#include "core/components/msim_decoder.h"
#include "core/components/msim_line.h"
#include "shared/svg_loader.h"

namespace gui::components {
    class msim_decoder_widget : public  QGraphicsSvgItem
                                , public msim_component_view
    {
        Q_OBJECT
    public:
        explicit msim_decoder_widget(
            core::components::msim_decoder* decoder,
            shared::svg_loader * loader,
            QString const & element_id,
            QGraphicsSvgItem * parent = nullptr
            );

        virtual QGraphicsProxyWidget * attach_to_target(gui::drop_target * target) override;

        virtual void update_display() override;
        virtual void show_tooltip() override;
        virtual void hide_tooltip() override;

        virtual QString id() const override;
        virtual QString label()const override;

    private:
        core::components::msim_decoder * m_decoder;
        QString m_element_id;
        QSvgRenderer * m_renderer;

    };
}

#endif //MICROSIMULATION_MSIM_DECODER_WIDGET_H