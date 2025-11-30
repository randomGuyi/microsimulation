#include "msim_alu.h"
#include "msim_ar.h"
#include "msim_bus.h"
#include "msim_cop.h"
#include "msim_cpu.h"
#include "msim_ram.h"
#include "msim_rom.h"
#include "../../shared/architecture_ids.h"
#include "core/components/msim_clock.h"
#include "core/components/msim_register.h"


#define DEBUG_CPU
#define DEBUG(msg) \
std::cout << "CPU: " << msg << std::endl

msim_cpu::msim_cpu()
    : m_curr_word{nullptr}
{}

/* errors */
void msim_cpu::clear_errors(){
    m_errors.clear();
}

void msim_cpu::component_error(std::string const & msg){
    m_errors.push_back("[cpu component error] " + msg);
}

std::vector<std::string> const & msim_cpu::get_errors() const{
    return m_errors;
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
            if(m_curr_word == nullptr){
                load_instruction();
            }
     //       execute_cycle();
            break;
        }
        case clock_event::PREV_CYCLE: {
            if(m_curr_word == nullptr){
                #ifdef DEBUG_CPU
                    DEBUG("TODO: implement error label !" );
                #endif
                return;
            }
        //    undo();
//            execute_cycle();
            std::cout << "shit, cycle called " << std::endl;
            break;
        }
        case clock_event::NEXT_PHASE: {
            if(m_curr_word == nullptr){
                load_instruction();
            }
            clock_phase curr_phase = clock->get_current_phase();
            execute_phase(clock->get_current_phase());
            break;
        }
        case clock_event::PREV_PHASE: {
            if(m_curr_word == nullptr){
            #ifdef DEBUG_CPU
                DEBUG("TODO: implement error label !" );
            #endif
                return;
            }
          //  undo_phase();
           // execute_phase(clock->get_current_phase());
            break;
        }
        case clock_event::MANUAL_MODE: {
            #ifdef DEBUG_CPU
                DEBUG("TODO: implement performace relevent features in manual mode (eg. reshow tooltips...)!" );
            #endif
            break;
        }
        case clock_event::AUTO_MODE: {
        #ifdef DEBUG_CPU
            DEBUG("TODO: implement performace relevent features in auto mode (eg. hide tooltips...)!" );
        #endif
            break;
        }
        case clock_event::START: {
            break;
        }
        case clock_event::STOP: {
            break;
        }
    }
}

/* ################################## PHASES  ##################################*/

void msim_cpu::load_instruction(){
    /* [car] -> cdr */
    auto * car = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERCAR));
    if(!car) return;

    auto * cdr = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTERCDR));
    if(! cdr) return;

    msim_rom * rom_inst = dynamic_cast<msim_rom *>(find_component(ID_COMP_ROM));
    if(! rom_inst){
        component_error("Rom");
        return;
    }
    if(! rom_inst->has_next()){ return;}

    int line_nr = car->getValue();
    if(! rom_inst->is_valid_line(line_nr)){
        // TODO: error mechanism !!!
    }
    rom_inst->reset_to_line(line_nr);
    m_curr_word = rom_inst->get_current_instruction();

//    cdr->setValue(m_curr_word->raw_word());

    fetch_word * fetch_command = m_curr_word->get_fetch_ops();

    decode_word * decode_command = m_curr_word->get_decode_ops();
    addrr_word * addr_command = m_curr_word->get_ar_ops();

    exec_word * execute_command = m_curr_word->get_exec_ops();

    set_fetch_instructions(fetch_command);
    set_decode_instructions(decode_command, addr_command);
    set_execute_instructions(execute_command);


}

void msim_cpu::read_from_ram(){
    if(! m_curr_word) return;
    if(! m_curr_word->get_read()) return;

    auto * ram = dynamic_cast<msim_ram *>(find_component(ID_COMP_RAM));
    if(! ram) return;

    auto * mar = dynamic_cast<msim_register *> (find_component(ID_COMP_REGISTERMAR));
    if(! mar) return;

    auto * mdr = dynamic_cast<msim_register *> (find_component(ID_COMP_REGISTERMDR));
    if(! mdr) return;

    mdr->setValue(ram->get_val_at(mar->getValue()));
}

void msim_cpu::fetch(){
    /* TODO: Connectors */
}

void msim_cpu::decode(){
}

void msim_cpu::execute(){
    /* reset word in order to load next cycle */
    m_curr_word = nullptr;

}

void msim_cpu::write_to_ram(){

}

/* ################################## SET INSTRUCTIONS ##################################*/
void msim_cpu::set_fetch_instructions( fetch_word * fw){

    uint8_t x_sel = fw->get_x_sel();
    uint8_t y_sel = fw->get_y_sel();
    uint8_t mdr_sel = fw->get_mdr_sel();

    auto set_bit_if = [=](  std::string const & bit_id
                          , bool set){
        if(! set){
            return false;
        }
        msim_bit * en_bit = m_enable_bits.at(bit_id).get();
        if(! en_bit){ return false; }

        en_bit->set_value(true);
        return true;
    };
/*
    if (set_bit_if(ID_ENBIT_REGISTER0_XBUS,  ((x_sel & FIRST_BIT) != 0)))
        goto Y_SEL;

    if(set_bit_if( ID_ENBIT_REGISTER1_XBUS, ((x_sel & SECOND_BIT) != 0)))
        goto Y_SEL;

    if(set_bit_if ( ID_ENBIT_REGISTER2_XBUS,   ((x_sel & THIRD_BIT) != 0)))
        goto Y_SEL;

    set_bit_if ( ID_ENBIT_REGISTER3_XBUS,  ((x_sel & FOURTH_BIT) != 0));

    Y_SEL :
    if (set_bit_if(ID_ENBIT_REGISTER0_YBUS, ((y_sel & FIRST_BIT) != 0)))
        goto MDR_SEL;

    if(set_bit_if( ID_ENBIT_REGISTER1_YBUS,  ((y_sel & SECOND_BIT) != 0)))
        goto MDR_SEL;

    if(set_bit_if ( ID_ENBIT_REGISTER2_YBUS,  ((y_sel & THIRD_BIT) != 0)))
        goto MDR_SEL;

    set_bit_if ( ID_ENBIT_REGISTER3_YBUS,  ((y_sel & FOURTH_BIT) != 0));

    MDR_SEL :
        set_bit_if(ID_ENBIT_REGISTERMDR_YBUS, ((mdr_sel & FIRST_BIT ) != 0));
        set_bit_if(ID_ENBIT_REGISTERMDR_COP, ((mdr_sel & SECOND_BIT) != 0));
    */
}

void msim_cpu::set_decode_instructions( decode_word * dw, addrr_word * aw){
    uint8_t operation = dw->get_operation();
    int cop_cn = aw->get_cn();
    int cop_mask = aw->get_mask();

    auto set_bit = [=](std::string const & bit_id, bool value){
        auto it = m_enable_bits.find(bit_id);
        if (it != m_enable_bits.end()){
            it->second.get()->set_value(value);
        }
    };

    /* update operation bits */
    set_bit(ID_ENBIT0_REGISTEROP, ((operation >> 0) & 0x1));
    set_bit(ID_ENBIT1_REGISTEROP, ((operation >> 1) & 0x1));
    set_bit(ID_ENBIT2_REGISTEROP, ((operation >> 2) & 0x1));
    set_bit(ID_ENBIT3_REGISTEROP, ((operation >> 3) & 0x1));

    msim_register * op_reg = dynamic_cast<msim_register *>(find_component(ID_COMP_REGISTEROP));
    if(! op_reg){
        component_error("OP REGISTER");
        return;
    }

    op_reg->setValue(operation);

    msim_cop * cop = dynamic_cast<msim_cop *>(find_component(ID_COMP_COP));
    if(! cop){
        component_error("cop REGISTER");
        return;
    }

    /* update mask bits */
    set_bit(ID_MASKBIT0_AR, ((cop_mask & (int)1) != 0));
    set_bit(ID_MASKBIT1_AR, ((cop_mask & (int)10) != 0));
    set_bit(ID_MASKBIT2_AR, ((cop_mask & (int)100) != 0));
    set_bit(ID_MASKBIT3_AR, ((cop_mask & (int)1000) != 0));


    cop->set_mask(cop_mask);

    /* update cn bits */
    set_bit(ID_CNBIT0_AR, (cop_cn >> 0) & 0x1);
    set_bit(ID_CNBIT1_AR, (cop_cn >> 1) & 0x1);
    set_bit(ID_CNBIT2_AR, (cop_cn >> 2) & 0x1);
    set_bit(ID_CNBIT3_AR, (cop_cn >> 3) & 0x1);

    cop->set_cn(cop_cn);
}

void msim_cpu::set_execute_instructions( exec_word * ew){
    uint8_t z_sel = ew->get_nibble();

    auto set_bit_if = [=](  std::string const & bit_id
                          , bool set){
        if(! set){
            return false;
        }
        msim_bit * en_bit = m_enable_bits.at(bit_id).get();
        if(! en_bit){ return false; }

        en_bit->set_value(true);
        return true;
    };
/*
    set_bit_if(ID_ENBIT_ZBUS_REGISTER0, ((z_sel & FIRST_BIT) != 0));
    set_bit_if(ID_ENBIT_ZBUS_REGISTER1, ((z_sel & SECOND_BIT) != 0));
    set_bit_if(ID_ENBIT_ZBUS_REGISTER2, ((z_sel & THIRD_BIT) != 0));
    set_bit_if(ID_ENBIT_ZBUS_REGISTER3, ((z_sel & FOURTH_BIT) != 0));
    */
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

