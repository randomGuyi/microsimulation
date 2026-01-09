/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MSIM_BIT_WIDGET_H
#define MSIM_BIT_WIDGET_H

#include "../../core/components/msim_bit.h"
#include "msim_component_svg_widget.h"
#include "msim_line_widget.h"

#include <QLabel>

namespace gui::components {
    class msim_bit_widget : public msim_component_svg_widget {
        Q_OBJECT
        public: explicit msim_bit_widget(core::components::msim_bit * bit,
                                         shared::svg_loader * loader,
                                         QString const & element_id,
                                         QWidget * parent = nullptr);

        virtual void add_line(msim_line_widget * line);
        virtual void remove_line(QString id);
        virtual void remove_line(msim_line_widget * line);
        /* TODO: implement Bit-specific functions */
    protected slots:
        void on_core_value_changed(bool value);

    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;

        core::components::msim_bit * m_bit;
        QMap<QString, msim_line_widget *> m_lines;
        QLabel * m_label;
        bool m_current_value;

    };
}
#endif // MSIM_BIT_WIDGET
