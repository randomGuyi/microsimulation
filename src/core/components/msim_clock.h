/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MSIM_CLOCK_H
#define MSIM_CLOCK_H

#include "core/components/msim_component.h"
#include "msim_observable_component.h"
#include <atomic>
#include <string>
#include <thread>

#include "msim_bus.h"

namespace core::components {
    enum class clock_event{

        NEXT_CYCLE,
        PREV_CYCLE,
        NEXT_PHASE,
        PREV_PHASE,

        MANUAL_MODE,
        AUTO_MODE,

        START,
        STOP
    };

    enum class clock_phase {
        FETCH = 0,
        DECODE = 1,
        EXECUTE = 2
    };

    class msim_clock final : public msim_component,
                             public msim_observable_component<clock_event>{
    public:
        msim_clock(std::string const & id, std::string const & label);
        void set_command(clock_event event);

        clock_phase get_current_phase() const;

        bool is_auto_mode() const;
        bool is_manual_mode() const;
        bool is_stop() const;

        bool is_running() const;

        void set_frequency(int frequency);
        ~msim_clock() override;

        void reset() override ;

    private:
        int m_frequency; /* cycles per secound */
        clock_phase m_phase_index; /* 0 = fetch, 1= decode, 2= execute */

        void on_next_cycle();
        void on_prev_cycle();
        void on_next_phase();
        void on_prev_phase();
        void on_manual_mode();
        void on_auto_mode();
        void on_start();
        void on_stop();

        int calc_interval_ms() const;
        void on_timer_tick();

        std::thread m_worker;
        std::atomic<bool> m_stop_flag;
        std::atomic<bool> m_auto_mode;
        std::atomic<bool> m_stop;
        std::atomic<bool> m_running;

    };
}
#endif // MSIM_CLOCK_H
