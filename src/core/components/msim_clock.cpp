#include <algorithm>
#include <cmath>

#include "msim_clock.h"


#define DEBUG_CLOCK

#ifdef DEBUG_CLOCK
#include <qDebug>
#endif

using namespace core::components;

msim_clock::msim_clock(std::string const & id, std::string const & label)
    : msim_component{id, label}
    , m_auto_mode{false}
    , m_stop{true}
    , m_running{false}
    , m_frequency{0}
    , m_phase_index{clock_phase::FETCH}
    , m_stop_flag{true}
{

}

clock_phase msim_clock::get_current_phase() const{
    return m_phase_index;
}

int msim_clock::calc_interval_ms() const{
    int freq = std::max(1, m_frequency);
    double phase_ms = 1000.0 / double(freq) / 3.0;
    return std::max(1, int(std::lround(phase_ms)));
}

void msim_clock::set_command(clock_event event){
    switch(event){
        case clock_event::NEXT_CYCLE:{
            on_next_cycle();
            break;
        }
        case clock_event::PREV_CYCLE:{
            on_prev_cycle();
            break;
        }
        case clock_event::NEXT_PHASE:{
            on_next_phase();
            break;
        }
        case clock_event::PREV_PHASE:{
            on_prev_phase();
            break;
        }
        case clock_event::MANUAL_MODE:{
            on_manual_mode();
            break;
        }
        case clock_event::AUTO_MODE:{
            on_auto_mode();
            break;
        }
        case clock_event::START:{
            on_start();
            break;
        }
        case clock_event::STOP:{
            on_stop();
            break;
        }
        default:
            /* nothing todo */
            break;

        }

}

void msim_clock::on_next_cycle(){
#ifdef DEBUG_CLOCK
    qDebug() << "clock_event::NEXT_CYCLE";
#endif
    if(! m_auto_mode){
        notify(clock_event::NEXT_CYCLE);
    }else{
        on_manual_mode();
        notify(clock_event::NEXT_CYCLE);
#ifdef DEBUG_CLOCK
        qWarning() << "set to manual mode, widget and core where out of sync ...";
#endif
    }
}

void msim_clock::on_prev_cycle(){
#ifdef DEBUG_CLOCK
            qDebug() << "clock_event::PREV_CYCLE";
#endif
    if(! m_auto_mode){
        notify(clock_event::PREV_CYCLE);
    }else{
        on_manual_mode();
        notify(clock_event::PREV_CYCLE);
#ifdef DEBUG_CLOCK
        qWarning() << "set to manual mode, widget and core where out of sync ...";
#endif
    }
}

void msim_clock::on_next_phase(){
#ifdef DEBUG_CLOCK
            qDebug() << "clock_event::NEXT_PHASE";
#endif
    if(! m_auto_mode){
        notify(clock_event::NEXT_PHASE);
    }else{
        on_manual_mode();
        notify(clock_event::NEXT_PHASE);
#ifdef DEBUG_CLOCK
        qWarning() << "set to manual mode, widget and core where out of sync ...";
#endif
    }
    m_phase_index =
        static_cast<clock_phase>(
        (static_cast<int>(m_phase_index) + 1) % 3);
}

void msim_clock::on_prev_phase(){
#ifdef DEBUG_CLOCK
            qDebug() << "clock_event::PREV_PHASE";
#endif
    if(! m_auto_mode){
        notify(clock_event::PREV_PHASE);
    }else{
        on_manual_mode();
        notify(clock_event::PREV_PHASE);
#ifdef DEBUG_CLOCK
        qWarning() << "set to manual mode, widget and core where out of sync ...";
#endif
    }
}

void msim_clock::on_manual_mode(){
#ifdef DEBUG_CLOCK
    qDebug() << "clock_event::MANUAL_MODE";
#endif
    m_auto_mode = false;
}

void msim_clock::on_auto_mode(){
#ifdef DEBUG_CLOCK
    qDebug() << "clock_event::AUTO_MODE";
#endif
    m_auto_mode = true;
}

void msim_clock::on_start(){
#ifdef DEBUG_CLOCK
    qDebug() << "clock_event::START";
#endif
    if(! m_auto_mode){
#ifdef DEBUG_CLOCK
        qDebug() << " clock out of sync with widget, start clicked in manual mode";
#endif
        on_auto_mode();
    }
    m_stop_flag = false;
    m_stop = false;
    m_running = true;
    m_phase_index = clock_phase::FETCH;

    m_worker = std::thread([this]() mutable{
        while (!m_stop_flag.load()) {
            int interval_ms = calc_interval_ms();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
            if(! m_stop_flag.load()){
                this->on_timer_tick();
            }
        }
    });
}

void msim_clock::on_stop(){
#ifdef DEBUG_CLOCK
    qDebug() << "clock_event::STOP";
#endif

    if(! m_auto_mode){
#ifdef DEBUG_CLOCK
        qDebug() << " clock out of sync with widget, start clicked in manual mode";
#endif
        on_auto_mode();
    }
    m_stop_flag = true;
    m_stop = true;
    m_running = false;

    if(m_worker.joinable()){
        m_worker.join();
    }
}


bool msim_clock::is_auto_mode(){
    return m_auto_mode;
}

bool msim_clock::is_manual_mode(){
    return ! m_auto_mode;
}

bool msim_clock::is_stop(){
    return m_stop;
}

bool msim_clock::is_running(){
    return m_running;
}

void msim_clock::set_frequency(int frequency){
    if(frequency < 1 ) frequency = 1;
    m_frequency = frequency;
    /* if the clock is running, the loop pause changes
     * automaticly, the worker is created each loop */
}

void msim_clock::on_timer_tick(){
    if(! m_running || !m_auto_mode) return;
    switch(m_phase_index){
        case clock_phase::FETCH:
        case clock_phase::DECODE: {
            notify(clock_event::NEXT_PHASE);
            break;
        }
        case clock_phase::EXECUTE:{
            notify(clock_event::NEXT_PHASE);
            notify(clock_event::NEXT_CYCLE);
            break;
        }
    }
        m_phase_index =
        static_cast<clock_phase>(
        (static_cast<int>(m_phase_index) + 1) % 3);
}

msim_clock::~msim_clock(){
    on_stop(); /* clean up thread */
#ifdef DEBUG_CLOCK
    qDebug() << "msim_clock desturctor called";
#endif
}
