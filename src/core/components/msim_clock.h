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

    class msim_clock : public msim_component,
                       public msim_observable_component<clock_event>{
    public:
        msim_clock(std::string const & id, std::string const & label);
        void set_command(clock_event event);

        clock_phase get_current_phase() const;

        bool is_auto_mode();
        bool is_manual_mode();
        bool is_stop();

        bool is_running();

        void set_frequency(int frequency);
        ~msim_clock();

        void reset() override {
            m_auto_mode = false;
            m_stop = true;
            m_running = false;
            m_frequency = 1;
            m_phase_index = clock_phase::FETCH;
            notify(clock_event::STOP);
        }

    private:
        bool m_auto_mode;
        bool m_stop;
        bool m_running;
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

    };
}
#endif // MSIM_CLOCK_H
