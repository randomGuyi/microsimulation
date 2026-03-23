/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MSIM_CLOCK_WIDGET_H
#define MSIM_CLOCK_WIDGET_H

#include "msim_component_svg_widget.h"

#include <shared/svg_loader.h>
#include <core/components/msim_clock.h>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QPropertyAnimation>

namespace gui::components {
    enum class state{
        MANUAL,
        AUTO,
        STOP
    };

    class msim_clock_widget : public msim_component_svg_widget{
        Q_OBJECT
    public:
        msim_clock_widget(core::components::msim_clock * clock,
                          shared::svg_loader * loader,
                          QString const & element_id,
                          QWidget * parent = nullptr);

        void update_display() override;

        void show_tooltip() override;
        void hide_tooltip() override;

    protected:
        void prev_phase();
        void next_phase();

        void prev_cycle();
        void next_cycle();

        void set_manual_mode();
        void set_auto_mode();

        void start_stop();

        void adjust_frequency(int value);


        core::components::msim_clock * m_clock;

        /* Layout for manual and auto mode */
        QFrame * m_manual_box;
        QFrame * m_auto_box;

        /* switch manual and auto mode */
        QPushButton * m_manual_btn;
        QPushButton * m_auto_btn;

        /* current segment display */
        QLabel * m_segment_label;

        /* automatic controls */
        QPushButton * m_auto_start_stop_btn;
        QPropertyAnimation * m_auto_start_stop_anim;
        QSlider * m_auto_freq_slider;

        /* automatic lables */
        QLabel * m_auto_freq_label;

        /* manual controls */
        QPushButton * m_man_next_phase_btn;
        QPropertyAnimation * m_man_next_phase_anim;
        QPushButton * m_man_prev_phase_btn;
        QPropertyAnimation * m_man_prev_phase_anim;

        QPushButton * m_man_next_cycle_btn;
        QPropertyAnimation * m_man_next_cycle_anim;
        QPushButton * m_man_prev_cycle_btn;
        QPropertyAnimation * m_man_prev_cycle_anim;

      private:
        void apply_btn_animation(QPushButton * btn, QPropertyAnimation * anim);
        QPropertyAnimation * setup_animation(QPushButton * btn);
    };
}
#endif // MSIM_CLOCK_WIDGET_H
