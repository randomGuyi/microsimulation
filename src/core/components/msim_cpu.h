#ifndef MSIM_CPU_H
#define MSIM_CPU_H

#include "core/components/msim_clock.h"
#include "core/components/msim_component.h"
#include "core/components/msim_connector.h"
#include "core/components/msim_enable_bit.h"
#include "core/microcode/inst_word.h"
#include <map>
#include <vector>

#define FIRST_BIT   0b00000001
#define SECOND_BIT  0b00000010
#define THIRD_BIT   0b00000100
#define FOURTH_BIT  0b00001000



namespace core::sim{

    struct cpu_error {
        int segment;
        std::string message;
    };


    enum class cpu_event_type {
        ERROR_OCCURRED = 1,
        RESET_COMPLETED = 2,
        NEXT_INSTRUCTION_LOADED = 3,
    };

    class msim_cpu: public components::msim_observable_component<std::pair<cpu_event_type, int>> {
    public:
        static msim_cpu & get_instance(){
            static msim_cpu instance{} ;
            return instance;
        }

        void reset_all();

        void add_component(std::unique_ptr<components::msim_component> && component);
        components::msim_component * get_component(std::string const & id);

        void add_connector(std::unique_ptr<components::msim_connector> && connector);
        components::msim_connector * get_connector(std::string const & id);

        void add_enable_bit(std::unique_ptr<components::msim_enable_bit> && enable_bit);
        components::msim_enable_bit * get_enable_bit(std::string const & id);

        [[nodiscard]] bool has_errors() const;

        [[nodiscard]] std::vector<cpu_error> const & get_errors() const;

    private:
        msim_cpu();
        ~msim_cpu() = default;
        msim_cpu(msim_cpu const & other) = delete;
        msim_cpu& operator=(msim_cpu const & other) = delete;


        void clear_errors();
        void component_error(std::string const & msg);

        void on_clock_changed(components::clock_event const & event);

        void set_fetch_instructions( const inst_word * wrd);
        void set_decode_instructions( const inst_word * wrd);
        void set_execute_instructions( const inst_word * wrd);

        //  void execute_cycle();
        void execute_phase(components::clock_phase const & phase);


        void load_instruction();
        void read_from_ram();

        void trasfer_data_to_buses();

        void transfer_data_to_registers();

        void fetch();

        std::pair<bool, int> execute_alu();

        void decode();
        void execute();
        void write_to_ram();


        components::msim_component * find_component(std::string const & id);

        std::map<std::string, std::unique_ptr<components::msim_component>> m_components;
        std::map<std::string, std::unique_ptr<components::msim_connector>> m_connectors;
        std::map<std::string, std::unique_ptr<components::msim_enable_bit>> m_enable_bits;
        inst_word const * m_curr_word;
        std::vector<cpu_error> m_errors;


    };
}
#endif // MSIM_CPU_H
