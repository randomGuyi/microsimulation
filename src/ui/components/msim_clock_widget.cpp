
#include "msim_clock_widget.h"
#include "../../shared/constants.h"
#include "ui/styles.h"
#include "shared/architecture_ids.h"

#include <QGraphicsOpacityEffect>
#include <QGroupBox>
#include <QPropertyAnimation>
#include <QTimer>
#include <QVBoxLayout>

  ////#define ID_CLOCK_SEG_DISPLAY "cycleDisp_clock"
 //#define ID_CLOCK_CONTROL_FRAME "controlDisp_clock"
 //#define ID_CLOCK_FETCH_DISP "fetchFrame_clock"
 //#define ID_CLOCK_DECODE_DISP "decodeFrame_clock"
 //#define ID_CLOCK_EXECUTE_DISP "executeFrame_clock"

msim_clock_widget::msim_clock_widget(msim_clock * clock,
                                     shared::svg_loader * loader,
                    QString const & element_id,
                                 QWidget * parent)
        : msim_component_svg_widget{loader, element_id, clock, parent}
    , m_clock{clock}
{
    /* Outer layout in order to host children */
    auto *outer_layout = new QVBoxLayout(this);
    outer_layout->setContentsMargins(0, 0, 0, 0);
    outer_layout->setSpacing(0);

    /* resize the clock widget and retrive a size / position to place the childs */
    QRectF clock_bounds = m_loader->get_renderer().boundsOnElement(element_id);
    setFixedSize(clock_bounds.size().toSize());

    /* adjust svg sub-elements to local coordinates */
    QRectF seg_rect  = m_loader->get_renderer().boundsOnElement(ID_CYCLEDISP_CLOCK);
    QRectF ctrl_rect = m_loader->get_renderer().boundsOnElement(ID_CONTROLDISP_CLOCK);
    seg_rect.translate(-clock_bounds.topLeft() + QPointF{0, 41.804187});
    ctrl_rect.translate(-clock_bounds.topLeft() + QPointF{0, 41.804187});

    /* Display to show the current segment */
    m_segment_label = new QLabel("# 0", this);
    m_segment_label->setAlignment(Qt::AlignCenter);
    m_segment_label->setStyleSheet(CLOCK_CYCLE_LABEL_STYLE);
    m_segment_label->setGeometry(seg_rect.toRect());

    /* control area */
    auto ctrl_panel = new QWidget(this);
    ctrl_panel->setGeometry(ctrl_rect.toRect());
    ctrl_panel->setContentsMargins(0, 0, 0, 0);

    auto *main_vbox = new QVBoxLayout(ctrl_panel);
    main_vbox->setContentsMargins(2,2,2,2);
    main_vbox->setSpacing(2);

    /* ########################  Mode buttons  ######################## */
    auto *mode_hbox = new QHBoxLayout;
    mode_hbox->setSpacing(2);

    m_manual_btn = new QPushButton("manual");
    m_manual_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_ENABLE_STYLE);
    m_manual_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_auto_btn   = new QPushButton("auto");
    m_auto_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_DISABLE_STYLE);
    m_auto_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mode_hbox->addWidget(m_manual_btn);
    mode_hbox->addWidget(m_auto_btn);
    main_vbox->addLayout(mode_hbox);

    /* ########################   Auto-mode controls  ######################## */
    auto *auto_box = new QFrame(ctrl_panel);
    auto_box->setContentsMargins(0, 0, 0, 0);

    auto_box->setFrameShape(QFrame::StyledPanel);

    auto_box->setVisible(false); /* initially hidden */
    auto *auto_vbox = new QVBoxLayout(auto_box);
    auto_vbox->setSpacing(1);
    auto_vbox->setContentsMargins(1,1,1,20);

    m_auto_start_stop_btn = new QPushButton("Start");
    m_auto_start_stop_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_ENABLE_STYLE);

    auto_vbox->addWidget(m_auto_start_stop_btn);

    m_auto_freq_label = new QLabel("Freq: 1 Hz");
    m_auto_freq_label->setStyleSheet(CLOCK_CYCLE_LABEL_STYLE);
    m_auto_freq_label->setAlignment(Qt::AlignCenter);

    m_auto_freq_slider = new QSlider(Qt::Horizontal);
    m_auto_freq_slider->setStyleSheet(CLOCK_FREQ_SLIDER_STYLE);
    m_auto_freq_slider->setRange(1, MAX_CLOCK_FREQUENCE);
    m_auto_freq_slider->setValue(1);

    auto_vbox->addWidget(m_auto_freq_label);
    auto_vbox->addWidget(m_auto_freq_slider);
    main_vbox->addWidget(auto_box);

    /* ########################   Manual-mode controls  ######################## */
    auto *manual_box = new QFrame(ctrl_panel);
    manual_box->setContentsMargins(0, 0, 0, 0);

    manual_box->setVisible(true);       /* initially shown */

    auto *manual_vbox = new QVBoxLayout(manual_box);
    manual_vbox->setSpacing(1);
    manual_vbox->setContentsMargins(1,1, 1, 1);

    /* phase controls */
    auto * phase_group = new QGroupBox("Phase");
    phase_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    phase_group->setContentsMargins(1, 1, 1, 1);      // we’ll position by geometry
    phase_group->setStyleSheet("QGroupBox { border: 1px solid orange; border-radius: 2px; margin-top: 1ex; font-size: 4px; } QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 1px; }");

    auto *phase_vbox = new QVBoxLayout{phase_group};
    phase_vbox->setContentsMargins(1,1,1,1);
    phase_vbox->setSpacing(0);

    m_man_prev_phase_btn = new QPushButton("Prev Phase");
    m_man_prev_phase_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_ENABLE_STYLE);
    m_man_prev_phase_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_man_prev_phase_btn->setMinimumHeight(8);

    m_man_next_phase_btn = new QPushButton("Next Phase");
    m_man_next_phase_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_ENABLE_STYLE);
    m_man_next_phase_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_man_next_phase_btn->setMinimumHeight(8);

    phase_group->setLayout(phase_vbox);
    phase_vbox->addWidget(m_man_prev_phase_btn);
    phase_vbox->addWidget(m_man_next_phase_btn);

    manual_vbox->addWidget(phase_group);

    /* Cycle control */
    auto * cycle_group = new QGroupBox{"Cycle"};
    cycle_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    cycle_group->setContentsMargins(0, 0, 0, 0);      // we’ll position by geometry
    cycle_group->setStyleSheet("QGroupBox { border: 1px solid orange; border-radius: 2px; margin-top: 1ex; font-size: 4px; } QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 1px; }");

    auto *cycle_vbox = new QVBoxLayout{cycle_group};
    cycle_vbox->setContentsMargins(1,1,1,1);
    cycle_vbox->setSpacing(0);

    m_man_prev_cycle_btn = new QPushButton("Prev Cycle");
    m_man_prev_cycle_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_ENABLE_STYLE);
    m_man_prev_cycle_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_man_prev_cycle_btn->setMinimumHeight(8);

    m_man_next_cycle_btn = new QPushButton("Next Cycle");
    m_man_next_cycle_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_ENABLE_STYLE);
    m_man_next_cycle_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_man_next_cycle_btn->setMinimumHeight(8);


    cycle_vbox->addWidget(m_man_prev_cycle_btn);
    cycle_vbox->addWidget(m_man_next_cycle_btn);

    manual_vbox->addWidget(cycle_group);

    main_vbox->addWidget(manual_box);

    /* ########################## Mode-switching logic ########################## */
    connect(m_manual_btn, &QPushButton::clicked, this, [=] {
        manual_box->setVisible(true);
        auto_box->setVisible(false);
        set_manual_mode();
    });
    connect(m_auto_btn, &QPushButton::clicked, this, [=] {
        manual_box->setVisible(false);
        auto_box->setVisible(true);
        set_auto_mode();
    });


    /* ########################## phase control logic ########################## */
    /* fade effects for all control buttons */
    /* auto creates variables of type lamda btw */
    auto setup_animation = [](QPushButton * btn) {
        auto *anim = new QPropertyAnimation(btn, "windowOpacity");
        anim->setDuration(500);
        anim->setStartValue(1.0);
        anim->setEndValue(0.0);
        anim->setEasingCurve(QEasingCurve::OutQuad);
        return anim;
    };

    /* automatic effects */
    QPropertyAnimation * auto_start_stop_anim = setup_animation(m_auto_start_stop_btn);
    /* manual animations */
    QPropertyAnimation * man_next_phase_anim = setup_animation(m_man_next_phase_btn);
    QPropertyAnimation * man_prev_phase_anim = setup_animation(m_man_prev_phase_btn);
    QPropertyAnimation * man_next_cycle_anim = setup_animation(m_man_next_cycle_btn);
    QPropertyAnimation * man_prev_cycle_anim = setup_animation(m_man_prev_cycle_btn);

  auto apply_btn_animation_fn = [=](QPushButton * btn,  QPropertyAnimation * anim){
        btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_DISABLE_STYLE);

        anim->stop();
        anim->start();

        QTimer::singleShot(500, [=](){
            btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_ENABLE_STYLE);
        });
    };

    connect(m_man_next_phase_btn, &QPushButton::clicked, this, [=]{
        apply_btn_animation_fn(m_man_next_phase_btn,  man_next_phase_anim);
        next_phase();
    });
    connect(m_man_prev_phase_btn, &QPushButton::clicked, this, [=]{
        apply_btn_animation_fn(m_man_prev_phase_btn,  man_prev_phase_anim);
        prev_phase();
    });
    connect(m_man_next_cycle_btn, &QPushButton::clicked, this, [=]{
        apply_btn_animation_fn(m_man_next_cycle_btn,  man_next_cycle_anim);
        next_cycle();
    });
    connect(m_man_prev_cycle_btn, &QPushButton::clicked, this, [=]{
        apply_btn_animation_fn(m_man_prev_cycle_btn, man_prev_cycle_anim);
        prev_cycle();
    });
    connect(m_auto_start_stop_btn, &QPushButton::clicked, this, [=]{
        apply_btn_animation_fn(m_auto_start_stop_btn, auto_start_stop_anim);
        start_stop();
    });

    connect(m_auto_freq_slider, &QSlider::valueChanged, this, [=](int value){
        m_auto_freq_label->setText(QString{"Freq: %1 Hz"}.arg(value));
        adjust_frequency(value);
    });
}

void msim_clock_widget::prev_phase(){
    m_clock->set_command(clock_event::PREV_PHASE);
}

void msim_clock_widget::next_phase(){
    m_clock->set_command(clock_event::NEXT_PHASE);
}

void msim_clock_widget::prev_cycle(){
    m_clock->set_command(clock_event::PREV_CYCLE);
}

void msim_clock_widget::next_cycle(){
    m_clock->set_command(clock_event::NEXT_CYCLE);
}

void msim_clock_widget::set_manual_mode(){
    m_auto_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_DISABLE_STYLE);
    m_manual_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_ENABLE_STYLE);
    m_clock->set_command(clock_event::MANUAL_MODE);
}

void msim_clock_widget::set_auto_mode(){
    m_clock->set_command(clock_event::AUTO_MODE);
    m_manual_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_DISABLE_STYLE);
    m_auto_btn->setStyleSheet(CLOCK_BTN_MANUAL_AUTO_ENABLE_STYLE);
}

void msim_clock_widget::start_stop(){
    if(m_clock->is_running()){
        m_clock->set_command(clock_event::STOP);
    }else{
        m_clock->set_command(clock_event::START);
    }
}


void msim_clock_widget::adjust_frequency(int value){
    m_clock->set_frequency(value);
}


void msim_clock_widget::update_display(){
    /* change appearance later on */
    update();
}
void msim_clock_widget::show_tooltip(){

}

void msim_clock_widget::hide_tooltip(){

}
