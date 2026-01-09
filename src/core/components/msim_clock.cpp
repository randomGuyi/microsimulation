/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
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
    , m_frequency{0}
    , m_phase_index{clock_phase::FETCH}
    , m_stop_flag{true}
    , m_auto_mode{false}
    , m_stop{true}
    , m_running{false}
{}

clock_phase msim_clock::get_current_phase() const{
    return m_phase_index;
}

int msim_clock::calc_interval_ms() const{
    const int freq = std::max(1, m_frequency);
    const double phase_ms = 1000.0 / static_cast<double>(freq) / 3.0;
    return std::max(1, static_cast<int>(std::lround(phase_ms)));
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
    if(! m_auto_mode.load()){
        notify(clock_event::NEXT_CYCLE);
    }else{
        /*  switch to manual mode before performing the manual-next action */
        /* if ui and core are out of sync */
        on_manual_mode();
        notify(clock_event::NEXT_CYCLE);
    }
}

void msim_clock::on_prev_cycle(){
    if(! m_auto_mode.load()){
        notify(clock_event::PREV_CYCLE);
    }else{
        on_manual_mode();
        notify(clock_event::PREV_CYCLE);
    }
}

void msim_clock::on_next_phase(){
    if(! m_auto_mode.load()){
        notify(clock_event::NEXT_PHASE);
    }else{
        on_manual_mode();
        notify(clock_event::NEXT_PHASE);
    }
    m_phase_index =
        static_cast<clock_phase>(
        (static_cast<int>(m_phase_index) + 1) % 3);
}

void msim_clock::on_prev_phase(){
    if(! m_auto_mode.load()){
        notify(clock_event::PREV_PHASE);
    }else{
        on_manual_mode();
        notify(clock_event::PREV_PHASE);
    }
}

void msim_clock::on_manual_mode(){
    m_auto_mode.store(false);
    notify(clock_event::MANUAL_MODE);
}

void msim_clock::on_auto_mode(){
    m_auto_mode.store(true);
    notify(clock_event::AUTO_MODE);
}

void msim_clock::on_start(){
    /* when starting, ensure we're in auto mode (start implies automatic stepping) */
    if(! m_auto_mode.load()){
        on_auto_mode();
    }
    m_stop_flag.store(false);
    m_stop.store(false);
    m_running.store(true);
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
    m_stop_flag.store(true);
    m_stop.store(true);
    m_running.store(false);

    if(m_worker.joinable()){
        m_worker.join();
    }
}


bool msim_clock::is_auto_mode() const {
    return m_auto_mode.load();
}

bool msim_clock::is_manual_mode() const {
    return ! m_auto_mode.load();
}

bool msim_clock::is_stop() const {
    return m_stop.load();
}

bool msim_clock::is_running() const {
    return m_running.load();
}

void msim_clock::set_frequency(int frequency){
    if(frequency < 1 ) frequency = 1;
    m_frequency = frequency;
    /* if the clock is running, the loop pause changes
     * automatically, the worker is created each loop */
}

void msim_clock::on_timer_tick(){
    if(! m_running.load() || !m_auto_mode.load()) return;
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
}

void msim_clock::reset() {
    m_stop_flag.store(true);
    if (m_worker.joinable()) {
        try {
            m_worker.join();
        } catch (...) {
            /* ignore */
        }
    }
    m_auto_mode.store(false);
    m_stop.store(true);
    m_running.store(false);
    m_frequency = 1;
    m_phase_index = clock_phase::FETCH;
}
