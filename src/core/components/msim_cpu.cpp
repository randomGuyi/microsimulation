/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#include "msim_ar.h"
#include "msim_cpu.h"
#include "msim_ram.h"
#include "msim_rom.h"
#include "../../shared/architecture_ids.h"
#include "core/components/msim_clock.h"
#include "core/components/msim_register.h"
#include <iostream>

#include "msim_alu.h"
#include "msim_bus.h"
#include "msim_cop.h"
#include "ui/components/debug_proxy.h"

using namespace core::components;
using namespace core::sim;

msim_cpu::msim_cpu()
    : m_curr_word{nullptr}
{}

/* ################################## ERROR HANDLING  ##################################*/
bool msim_cpu::has_errors() const{
    return ! m_errors.empty();
}
void msim_cpu::clear_errors(){
    m_errors.clear();
}

void msim_cpu::component_error(std::string const & msg){
    const auto * car = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERCAR));
    if(!car) {
        m_errors.push_back({0, "[cpu component error] CAR is missing"});
        notify({cpu_event_type::ERROR_OCCURRED, 0});
        return;
    }
    m_errors.push_back({car->getValue() / 4, "[cpu component error] " + msg});
    const auto * rom = dynamic_cast<msim_rom *>(find_component(ID_COMP_ROM));
    int segment = rom ? rom->get_current_line() : 0;
    notify({cpu_event_type::ERROR_OCCURRED, segment});
}

std::vector<cpu_error> const & msim_cpu::get_errors() const{
    return m_errors;
}
/* ################################## RESET  ##################################*/
void msim_cpu::reset_all() {
    for (auto & [id, comp] : m_components) {
        std::cout << "id" << id << std::endl;
        comp->reset();
    }
    for (auto & [id, conn] : m_connectors) {
        std::cout << "id" << id << std::endl;
        conn->reset();
    }
    for (auto & [id, bit] : m_enable_bits) {
        std::cout << "id" << id << std::endl;
        bit->reset();
    }
    m_curr_word = nullptr;
    m_errors.clear();
    auto const rom = dynamic_cast<msim_rom *>(find_component(ID_COMP_ROM));
    int segment = rom ? rom->get_current_line() : 0;

    notify({ cpu_event_type::RESET_COMPLETED,  segment});
}


/* ################################## CLOCK CONTROL  ##################################*/
void msim_cpu::execute_phase(clock_phase const & phase){

    switch(phase){
        case clock_phase::FETCH:{
            fetch();
            break;
        }
        case clock_phase::DECODE:{
            decode();
            break;
        }
        case clock_phase::EXECUTE:{
            execute();
            break;
        }
        default:{
            /* nothing todo */
            break;
        }
    }
}

void msim_cpu::on_clock_changed(clock_event const & event){
    msim_clock * clock = dynamic_cast<msim_clock *>(m_components.at(ID_COMP_CLOCK).get());
    switch(event){
        case clock_event::NEXT_CYCLE: {
            // TODO: implement execute cycle
            break;
        }
        case clock_event::PREV_CYCLE: {
            if(m_curr_word == nullptr)return;
            break;
            // TODO: implement previous cycle
        }
        case clock_event::NEXT_PHASE: {
            execute_phase(clock->get_current_phase());
            break;
        }
        case clock_event::PREV_PHASE: {
            if(m_curr_word == nullptr)return;
            // TODO: implement next phase
            break;
        }
        case clock_event::MANUAL_MODE:
        case clock_event::AUTO_MODE:
        case clock_event::START:
        case clock_event::STOP:
            // TODO: implement performace relevent features in manual mode (eg. show tooltips...)
            break;
        }
    }

/* ################################## PHASES  ##################################*/

void msim_cpu::load_instruction(){
    /* [car] -> cdr */
    const auto * car = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERCAR));
    if(!car) {
        component_error("CAR is missing");
        return;
    }

    auto * cdr = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERCDR));
    if(! cdr) {
        component_error("CDR is missing");
        return;
    }

    auto rom_inst = dynamic_cast<msim_rom *>(find_component(ID_COMP_ROM));
    if(! rom_inst){
        component_error("ROM is missing");
        return;
    }

    int line_nr = car->getValue();
    if(! rom_inst->is_valid_line(line_nr)){
        m_errors.push_back({ cdr->getValue() / 4, "[cpu] invalid line number requested from ROM, CAR[" + std::to_string(line_nr) + "]"});
        return;
    }
    rom_inst->reset_to_line(line_nr);
    m_curr_word = rom_inst->get_current_instruction();

    notify({cpu_event_type::NEXT_INSTRUCTION_LOADED, rom_inst->get_current_line()});

    /* set value in CDR, so the next to set are the bits */
    cdr->setValue(static_cast<int>(m_curr_word->get_raw_word()));

    set_fetch_instructions(m_curr_word);
    set_decode_instructions(m_curr_word);
    set_execute_instructions(m_curr_word);
}

void msim_cpu::read_from_ram(){
    if(! m_curr_word) return;
    if(! (m_curr_word->get_ram_mode() == ram_mode::READ)) return;

    auto * ram = dynamic_cast<msim_ram *>(find_component(ID_COMP_RAM));
    if(! ram) {
        component_error("RAM is missing"); // ram mode is read, but no ram was placed
        return;
    }

    auto * mar = dynamic_cast<msim_register *> (find_component(ID_COMP_REGISTERMAR));
    if(! mar) {
        component_error("MAR is missing");
        return;
    }

    auto * mdr = dynamic_cast<msim_register *> (find_component(ID_COMP_REGISTERMDR));
    if(! mdr) {
        component_error("MDR is missing");
        return;
    };

    mdr->setValue(ram->get_val_at(mar->getValue()));
}

void msim_cpu::transfer_data_to_buses() {

    auto transfer_if = [this] (std::string const & connector_id,std::string const & bus_id,
                               int value,                  bool enable) {

        /* set connectors */
        auto * conn = get_connector(connector_id);
        if(! conn) return ! enable;
        enable ? conn->enable() : conn->disable();
        /* transfer to bus */
        if(! enable) return true;
        auto * bus = dynamic_cast<msim_bus *>(find_component(bus_id));
        if(! bus) return false; /* bus must exist if its enabled */
        bus->set_value(value);
        return true;
    };

    /* transfer to x bus */
    auto * xbus = dynamic_cast<msim_bus *>(find_component(ID_COMP_XBUS));
    if(! xbus) return;

    if (! transfer_if(ID_PCB_REGISTER0_XBUS, ID_COMP_XBUS,
                dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTER0))->getValue(),
                m_curr_word->get_x_selection() == REGISTER_0)) {
        component_error("X BUS is missing");

    }

    if (! transfer_if(ID_PCB_REGISTER1_XBUS, ID_COMP_XBUS,
                dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTER1))->getValue(),
                m_curr_word->get_x_selection() == REGISTER_1)) {
        component_error("X BUS is missing");
    }

    if ( ! transfer_if(ID_PCB_REGISTER2_XBUS, ID_COMP_XBUS,
                dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTER2))->getValue(),
                m_curr_word->get_x_selection() == REGISTER_2)) {
        component_error("X BUS is missing");
    }

    if (! transfer_if(ID_PCB_REGISTER3_XBUS, ID_COMP_XBUS,
                dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTER3))->getValue(),
                m_curr_word->get_x_selection() == REGISTER_3)) {

        component_error("X BUS is missing");
    }


    /* transfer to y bus */
    auto * ybus = dynamic_cast<msim_bus *>(find_component(ID_COMP_YBUS));
    if(! ybus) return;

    if (! transfer_if(ID_PCB_REGISTER0_YBUS, ID_COMP_YBUS,
                dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTER0))->getValue(),
                m_curr_word->get_y_selection() == REGISTER_0)) {
        component_error("Y BUS is missing");
    }

    if (! transfer_if(ID_PCB_REGISTER1_YBUS, ID_COMP_YBUS,
                dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTER1))->getValue(),
                m_curr_word->get_y_selection() == REGISTER_1)) {
        component_error("Y BUS is missing");
    };

    if (! transfer_if(ID_PCB_REGISTER2_YBUS, ID_COMP_YBUS,
                dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTER2))->getValue(),
                m_curr_word->get_y_selection() == REGISTER_2)) {
        component_error("Y BUS is missing");
    }

    if (! transfer_if(ID_PCB_REGISTER3_YBUS, ID_COMP_YBUS,
                dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTER3))->getValue(),
                m_curr_word->get_y_selection() == REGISTER_3)) {
        component_error("Y BUS is missing");
    }

    if (! transfer_if(ID_PCB_REGISTERMDR_YBUS, ID_COMP_YBUS,
                dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERMDR))->getValue(),
                m_curr_word->get_mdr_y())) {
        component_error("Y BUS is missing");
    }
}

void msim_cpu::transfer_data_to_registers() {
    /* transfer operation */
    auto * op_register = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTEROP));
    if(op_register) {
        op_register->setValue(m_curr_word->get_operation());
    }

    /* x-bus to Register x */
    auto * xbus = dynamic_cast<msim_bus *>(find_component(ID_COMP_XBUS));
    auto * register_x = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERX));
    if (xbus && register_x) {
        register_x->setValue(xbus->get_value());
    }

    /* y-bus to Register y */
    auto * ybus = dynamic_cast<msim_bus *>(find_component(ID_COMP_YBUS));
    auto * register_y = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERY));
    if ((ybus != nullptr) && (register_y != nullptr)) {
        register_y->setValue(ybus->get_value());
    }

    /* mdr to cop */
    auto * cop = dynamic_cast<msim_cop *>(find_component(ID_COMP_COP));
    if (! cop) return;

    auto * mdr = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERMDR));
    if (! mdr) return;

    auto * pcb_mdr_cop = get_connector(ID_PCB_REGISTERMDR_COP);
    if (! pcb_mdr_cop) return;

    if (m_curr_word->get_mdr_cop()) {
        pcb_mdr_cop->enable() ;
        cop->setValue(mdr->getValue());
    }else {
        pcb_mdr_cop->disable();
    }
}

void msim_cpu::fetch(){
    /* update all bits */
    for (auto & [bit_id, bit_ptr] : m_enable_bits) bit_ptr->set_value(false);
    /* reset all the connectors from execute */
    for (auto & [con_id, con_ptr] : m_connectors)  con_ptr->disable();
    load_instruction();
    if (m_curr_word == nullptr) {
        return;
    }

    read_from_ram();
    transfer_data_to_buses();
    transfer_data_to_registers();
}

std::pair<bool, int> msim_cpu::execute_alu() {

    /* alu is the central part */
    auto * alu = dynamic_cast<msim_alu*>(find_component(ID_COMP_ALU));
    if (! alu) {
        component_error("ALU is missing");
        return {false, 0};
    }
    auto * opreg = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTEROP));
    if (! opreg) {
        component_error("OP REGISTER is missing");
        return {false, 0}; // no operation, nothing to do
    }

    int const_val = m_curr_word->get_constant_nbr(); // cheating
    alu->set_operation(static_cast<uint8_t>(opreg->getValue()));
    alu->set_constant_value(const_val);

    auto * x_reg = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERX));
    auto * y_reg = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERY));

    if (x_reg != nullptr && y_reg != nullptr) {
        alu->set_x_value(x_reg->getValue());
        alu->set_y_value(y_reg->getValue());
        alu->execute();
    }else if (x_reg != nullptr) {
        alu->set_x_value(x_reg->getValue());
        switch (opreg->getValue()) {
            case Z_Z:
            case Z_CONST:
            case Z_X:
            case Z_INC_X:
            case Z_DEC_X:
            case Z_MINUS_X: {
                alu->execute();
                break;
            }
            default: {
                component_error("Y Register is missing");
                return {false, 0};
            }
        }
    } else if (y_reg != nullptr) {
        alu->set_y_value(static_cast<int>(y_reg->getValue()));
        switch (opreg->getValue()) {
            case Z_Z:
            case Z_CONST:
            case Z_Y:
            case Z_INC_Y:
            case Z_DEC_Y: {
                alu->execute();
                break;
            }
            default: {
                component_error("X Register is missing");
                return {false, 0};
            }
        }
    } else {
        switch (opreg->getValue()) {
            case Z_Z:
            case Z_CONST:{
                alu->execute();
                break;
            }
            default: {
                component_error("X or Y Registers are missing");
                return {false, 0};
            }
        }
    }
    return {true, alu->get_result()};
}

void msim_cpu::decode(){
    /* reset all the connectors from fetch */
    for (auto & [con_id, con_ptr] : m_connectors)  con_ptr->disable();
    if (m_curr_word == nullptr) return;

    auto [result_ok, result_value] = execute_alu();
    if (! result_ok) {
        qDebug("ALU execution failed or no operation to perform");
        return;
    }
    /* transfer alu result to z bus */
    auto * zbus = dynamic_cast<msim_bus *>(find_component(ID_COMP_ZBUS));
    if (zbus) {
        zbus->set_value(result_value);
    }else if (m_curr_word->get_z_selection() > 0 || m_curr_word->get_z_mar() || m_curr_word->get_z_mdr()) {
        component_error("Z BUS is missing");
    }

    /* set z register */
    auto * z_reg = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERZ));
    if (z_reg) {
        z_reg->setValue(result_value);
    }else if (m_curr_word->get_z_selection() > 0 || m_curr_word->get_z_mar() || m_curr_word->get_z_mdr()) {
        component_error("Z REGISTER is missing");
    }

    /* set flags */
    auto * flags_register = dynamic_cast<msim_register*>(find_component(ID_COMP_REGISTERFLAGS));
    auto * alu = dynamic_cast<msim_alu*>(find_component(ID_COMP_ALU)); // must exist
    if (flags_register) {
        flags_register->setValue(alu->get_flags());
    };

    /* ar logic */
    auto * ar = dynamic_cast<msim_ar *>(find_component(ID_COMP_AR));
    if (! ar) {
        component_error("AR is missing");
        return;
    }

    auto * car = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERCAR));
    if (! car) {
        component_error("CAR is missing");
        return; // car must exist
    }

    switch (m_curr_word->get_ar_mode()) {
        case ar_mode::CHAR_PLS_PLS: {
            car->setValue(car->getValue() + 1);
            break;

        }case ar_mode::_4CN: {
            int cn_value = m_curr_word->get_cn();
            car->setValue(4 * cn_value);
            break;

        }case ar_mode::_4COP:{
            auto cop = dynamic_cast<msim_cop *>(find_component(ID_COMP_COP));
            if(! cop) {
                component_error("COP is missing");
                return;
            }
            int cop_value = cop->getValue();
            car->setValue(4 * cop_value);
            break;

        }case ar_mode::_4COP_CND: {
            uint8_t mask = m_curr_word->get_mask();
            uint8_t flags = alu->get_flags();
            if (mask & flags) {
                auto cop = dynamic_cast<msim_cop *>(find_component(ID_COMP_COP));
                if(! cop) return;
                int cop_value = cop->getValue();
                car->setValue(4 * cop_value);
            }else {
                car->setValue(car->getValue() + 1);
            }
            break;
        }
    }
}

void msim_cpu::execute(){
    if (m_curr_word == nullptr) return;
    auto trasfer_if = [this] (std::string const & register_id, std::string const & connector_id, int value, bool enable) {
        auto * reg = dynamic_cast<msim_register *>(find_component(register_id));
        if(! reg) {
            return ! enable;
        }
        auto * conn = get_connector(connector_id);
        if(! conn) return ! enable;
        enable ? conn->enable() : conn->disable();
        if(! enable) return true;
        reg->setValue(value);
        return true;
    };

    /* write z bus to selected register */
    auto * zbus = dynamic_cast<msim_bus *>(find_component(ID_COMP_ZBUS));
    if (zbus) {
        uint8_t z_selection = m_curr_word->get_z_selection();

       if ( ! trasfer_if(ID_COMP_REGISTER0, ID_PCB_ZBUS_REGISTER0,
           zbus->get_value(), (z_selection & 0b00001000) > 0)) {
              component_error("Register 0 is missing");
       }

        if ( ! trasfer_if(ID_COMP_REGISTER1, ID_PCB_ZBUS_REGISTER1,
            zbus->get_value(),(z_selection & 0b00000100) > 0)) {
                component_error("Register 1 is missing");
        }

        if ( ! trasfer_if(ID_COMP_REGISTER2, ID_PCB_ZBUS_REGISTER2,
            zbus->get_value(),(z_selection & 0b00000010) > 0)) {
            component_error("Register 2 is missing");
        }

        if ( ! trasfer_if(ID_COMP_REGISTER3, ID_PCB_ZBUS_REGISTER3,
            zbus->get_value(),(z_selection == 0b00000001)> 0) ) {
            component_error("Register 3 is missing");
        }

        if ( ! trasfer_if(ID_COMP_REGISTERMDR, ID_PCB_ZBUS_REGISTERMDR,
            zbus->get_value(),m_curr_word->get_z_mdr())) {
            component_error("Register mdr is missing");
        }

        if ( ! trasfer_if(ID_COMP_REGISTERMAR, ID_PCB_ZBUS_REGISTERMAR,
            zbus->get_value(),m_curr_word->get_z_mar())) {
            component_error("Register mar is missing");
        }
    }

    write_to_ram();

    /* reset word in order to load next cycle */
    m_curr_word = nullptr;
}

void msim_cpu::write_to_ram(){
    if(! m_curr_word) return;
    if(! (m_curr_word->get_ram_mode() == ram_mode::WRITE)) return;

    auto * ram = dynamic_cast<msim_ram *>(find_component(ID_COMP_RAM));
    if(! ram) {
        component_error("RAM is missing"); // ram mode is write, but no ram was placed
        return;
    }

    auto * mar = dynamic_cast<msim_register *> (find_component(ID_COMP_REGISTERMAR));
    if(! mar) {
        component_error("MAR is missing");
        return;
    }

    auto * mdr = dynamic_cast<msim_register *> (find_component(ID_COMP_REGISTERMDR));
    if(! mdr) {
        component_error("MDR is missing");
        return;
    }

    ram->set_val_at(mdr->getValue(), mar->getValue());
}

/* ################################## SET INSTRUCTIONS ##################################*/
void msim_cpu::set_fetch_instructions( const inst_word * word){

    /* 0 0 0 0 0 s1 s0 en */
     uint8_t const x_sel = word->get_x_selection();
    /* 0 0 0 0 0 s1 s0 en */
     uint8_t const y_sel = word->get_y_selection();


     auto set_bit_if = [this]( std::string const & bit_id, bool set){
         if(! set){
             return true;
         }
         msim_bit * en_bit = get_enable_bit(bit_id);
         if(! en_bit){ return  !set; }

         en_bit->set_value(true);
         return true;
     };

    /* set x bus bits */
    set_bit_if(ID_ENBIT_DEC_XBUS_REGISTERA, (x_sel & 0b00000001) > 0); // enable
    set_bit_if(ID_ENBIT0_XBUS_REGISTERA, (x_sel & 0b00000010) > 0);
    set_bit_if(ID_ENBIT1_XBUS_REGISTERA, (x_sel & 0b00000100) > 0 );

    /* set y bus bits */
    set_bit_if(ID_ENBIT_DEC_YBUS_REGISTERA, (y_sel & 0b00000001) > 0); // enable
    set_bit_if(ID_ENBIT0_YBUS_REGISTERA, (y_sel & 0b00000010) > 0);
    set_bit_if(ID_ENBIT1_YBUS_REGISTERA, (y_sel & 0b00000100) > 0 );

    /* set mdr to y bus */
    set_bit_if(ID_ENBIT_REGISTERMDR_YBUS, word->get_mdr_y());
    set_bit_if(ID_ENBIT_REGISTERMDR_COP, word->get_mdr_cop());

 }

 void msim_cpu::set_decode_instructions( const inst_word * word) {
     uint8_t operation = word->get_operation();
     int cn = word->get_cn();
      uint8_t mask = word->get_mask();

    auto set_bit_if = [this]( std::string const & bit_id, bool set){
        if(! set){
            return false;
        }
        msim_bit * en_bit = get_enable_bit(bit_id);
        if(! en_bit){ return false; }

        en_bit->set_value(true);
        return true;
    };

     /* update operation bits */
     set_bit_if(ID_ENBIT0_REGISTEROP, ((operation >> 0) & 0x1));
     set_bit_if(ID_ENBIT1_REGISTEROP, ((operation >> 1) & 0x1));
     set_bit_if(ID_ENBIT2_REGISTEROP, ((operation >> 2) & 0x1));
     set_bit_if(ID_ENBIT3_REGISTEROP, ((operation >> 3) & 0x1));

     /* update mask bits */
     set_bit_if(ID_MASKBIT0_AR, (mask & 0b00000001) > 0);
     set_bit_if(ID_MASKBIT1_AR, (mask & 0b00000010) > 0);
     set_bit_if(ID_MASKBIT2_AR, (mask & 0b00000100) > 0);
     set_bit_if(ID_MASKBIT3_AR, (mask & 0b00001000) > 0);

    /* update ram mode */
     switch (m_curr_word->get_ram_mode()) {
         case ram_mode::READ: {
             set_bit_if(ID_MODEBIT0_REGISTERMODE, true);
             break;
         }
         case ram_mode::WRITE: {
             set_bit_if(ID_MODEBIT1_REGISTERMODE, true);
             break;
         }
         default: /* nothing to do, WAIT */
             break;
     }

    /* update ar mode */
    switch (m_curr_word->get_ar_mode()) {
         case ar_mode::CHAR_PLS_PLS: {
             break; // nothing to do
             }
         case ar_mode::_4CN: {
             set_bit_if(ID_MODEBIT0_AR, true);
             break;
         }
         case ar_mode::_4COP: {
             set_bit_if(ID_MODEBIT1_AR, true);
             break;
         }
         case ar_mode::_4COP_CND: {
              set_bit_if(ID_MODEBIT0_AR, true);
              set_bit_if(ID_MODEBIT1_AR, true);
             break;
         }
    }

    /* update cn bits */
     set_bit_if(ID_CNBIT0_AR, (cn >> 0) & 0x1);
     set_bit_if(ID_CNBIT1_AR, (cn >> 1) & 0x1);
     set_bit_if(ID_CNBIT2_AR, (cn >> 2) & 0x1);
     set_bit_if(ID_CNBIT3_AR, (cn >> 3) & 0x1);
 }

 void msim_cpu::set_execute_instructions( const inst_word * wrd){
     uint8_t z_sel = wrd->get_z_selection();

    auto set_bit_if = [this]( std::string const & bit_id, bool set){
        if(! set){
            return false;
        }
        msim_bit * en_bit = get_enable_bit(bit_id);
        if(! en_bit){ return false; }

        en_bit->set_value(true);
        return true;
    };

    /* set z bus bits */
    set_bit_if(ID_ENBIT_ZBUS_REGISTER0, (z_sel & 0b00001000) > 0);
    set_bit_if(ID_ENBIT_ZBUS_REGISTER1, (z_sel & 0b00000100) > 0);
    set_bit_if(ID_ENBIT_ZBUS_REGISTER2, (z_sel & 0b00000010) > 0);
    set_bit_if(ID_ENBIT_ZBUS_REGISTER3, (z_sel & 0b00000001) > 0);

    set_bit_if(ID_ENBIT_ZBUS_REGISTERMAR, wrd->get_z_mar());
    set_bit_if(ID_ENBIT_ZBUS_REGISTERMDR, wrd->get_z_mdr());

}


/* ################################## COMPONENT MANAGEMENT ##################################*/
msim_component * msim_cpu::find_component(std::string const & id){
    auto inst_it = m_components.find(id);
    if(inst_it == m_components.end()){
        std::cout << " implement a mechanism to check if all components are placed " << std::endl;
        return nullptr;
    }
    return inst_it->second.get();
}


void msim_cpu::add_component(std::unique_ptr<msim_component> && component){

    if(m_components.find(component->id()) != m_components.end()){
        throw std::runtime_error("cpu already contains component" + component->id() + " ");
    }
    if(component->id() == ID_COMP_CLOCK){
        m_components.insert({component->id(), std::move(component)});
        msim_clock * clk = dynamic_cast<msim_clock *>(m_components.at(ID_COMP_CLOCK).get());
        clk->subscibe([this](clock_event event){
            this->on_clock_changed(event);
        });
    }else{
        m_components.insert({component->id(), std::move(component)});
    }
}

msim_component * msim_cpu::get_component(std::string const & id){
    auto it = m_components.find(id);
    return it == m_components.end() ? nullptr : it->second.get();
}

void msim_cpu::add_connector(std::unique_ptr<msim_connector> && connector){
    if(m_connectors.find(connector->id()) != m_connectors.end()){
        throw std::runtime_error("cpu already contains component" + connector->id() + " ");
    }
    m_connectors.insert({connector->id(), std::move(connector)});
}

msim_connector * msim_cpu::get_connector(std::string const & id){
    auto it = m_connectors.find(id);
    return it == m_connectors.end() ? nullptr : it->second.get();
}

void msim_cpu::add_enable_bit(std::unique_ptr<msim_enable_bit> && enable_bit){
    if(m_enable_bits.find(enable_bit->id()) != m_enable_bits.end()){
        throw std::runtime_error("cpu already contains enable bit " + enable_bit->id() + " ");
    }
    m_enable_bits.insert({enable_bit->id(),std::move(enable_bit)});
}

msim_enable_bit * msim_cpu::get_enable_bit(std::string const & id){
    auto it = m_enable_bits.find(id);
    return it == m_enable_bits.end() ? nullptr : it->second.get();
}
