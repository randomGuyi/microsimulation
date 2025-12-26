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

class msim_cpu{
public:
    static msim_cpu & get_instance(){
        static msim_cpu instance{} ;
        return instance;
    }


    void add_component(std::unique_ptr<msim_component> && component);
    msim_component * get_component(std::string const & id);

    void add_connector(std::unique_ptr<msim_connector> && connector);
    msim_connector * get_connector(std::string const & id);

    void add_enable_bit(std::unique_ptr<msim_enable_bit> && enable_bit);
    msim_enable_bit * get_enable_bit(std::string const & id);

    std::vector<std::string> const & get_errors() const;



private:
    msim_cpu();
    ~msim_cpu() = default;
    msim_cpu(msim_cpu const & other) = delete;
    msim_cpu& operator=(msim_cpu const & other) = delete;

    inst_word const * m_curr_word;
    std::vector<std::string> m_errors;

    void clear_errors();
    void component_error(std::string const & msg);

    void on_clock_changed(clock_event const & event);

    void set_fetch_instructions( const inst_word * wrd);
    void set_decode_instructions( const inst_word * wrd);
    void set_execute_instructions( const inst_word * wrd);

  //  void execute_cycle();
    void execute_phase(clock_phase const & phase);


    void load_instruction();
    void read_from_ram();

    void trasfer_data_to_buses();

    void transfer_data_to_registers();

    void fetch();

    std::pair<bool, int> execute_alu();

    void decode();
    void execute();
    void write_to_ram();


    msim_component * find_component(std::string const & id);

    std::map<std::string, std::unique_ptr<msim_component>> m_components;
    std::map<std::string, std::unique_ptr<msim_connector>> m_connectors;
    std::map<std::string, std::unique_ptr<msim_enable_bit>> m_enable_bits;


};

#endif // MSIM_CPU_H
