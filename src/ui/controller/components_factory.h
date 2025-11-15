#ifndef COMPONENTS_FACTORY_H
#define COMPONENTS_FACTORY_H

#include "core/components/msim_alu.h"
#include "core/components/msim_ar.h"
#include "core/components/msim_bus.h"
#include "core/components/msim_clock.h"
#include "core/components/msim_component.h"
#include "core/components/msim_cop.h"
#include "core/components/msim_ram.h"
#include "core/components/msim_register.h"
#include "core/components/msim_rom.h"

#include "ui/components/msim_alu_widget.h"
#include "ui/components/msim_ar_widget.h"
#include "ui/components/msim_bus_widget.h"
#include "ui/components/msim_clock_widget.h"
#include "ui/components/msim_component_widget.h"
#include "ui/components/msim_cop_widget.h"
#include "ui/components/msim_ram_widget.h"
#include "ui/components/msim_register_widget.h"
#include "ui/components/msim_rom_widget.h"

#include "shared/svg_loader.h"

#include <core/components/msim_cpu.h>

class components_factory
{
public:
    static std::pair<msim_component *, msim_component_widget *>
    create(const QString & id, const QString & label, shared::svg_loader * loader){
        if(id.contains("alu")){
            auto & cpu = msim_cpu::get_instance();
            cpu.add_component(std::make_unique<msim_alu>(id.toStdString(), label.toStdString()));

            msim_alu * comp_ptr = dynamic_cast< msim_alu *>(cpu.get_component(id.toStdString()));
            msim_alu_widget * widget = new msim_alu_widget{comp_ptr, loader, id};
            return {msim_cpu::get_instance().get_component(id.toStdString()), widget};

        }else if(id.contains("register")){
            auto & cpu = msim_cpu::get_instance();
            cpu.add_component(std::make_unique<msim_register>(id.toStdString(), label.toStdString()));

            msim_register *  comp_ptr = dynamic_cast< msim_register *>(cpu.get_component(id.toStdString()));
            msim_register_widget * widget = new msim_register_widget{comp_ptr, label};
            return{msim_cpu::get_instance().get_component(id.toStdString()), widget};
        }else if (id.contains("Bus")){
            auto & cpu = msim_cpu::get_instance();
            cpu.add_component(std::make_unique<msim_bus>(id.toStdString(), label.toStdString()));

            msim_bus *  comp_ptr = dynamic_cast< msim_bus *>(cpu.get_component(id.toStdString()));
            msim_bus_widget * widget = new msim_bus_widget{comp_ptr, loader, id};
            return{msim_cpu::get_instance().get_component(id.toStdString()), widget};
        }else if (id.contains("ram")){
            auto & cpu = msim_cpu::get_instance();
            cpu.add_component(std::make_unique<msim_ram>(id.toStdString(), label.toStdString()));

            msim_ram *  comp_ptr = dynamic_cast< msim_ram *>(cpu.get_component(id.toStdString()));
            msim_ram_widget * widget = new msim_ram_widget{comp_ptr, loader, id};
            return{msim_cpu::get_instance().get_component(id.toStdString()), widget};
        }else if (id.contains("rom")){
            auto & cpu = msim_cpu::get_instance();
            cpu.add_component(std::make_unique<msim_rom>(id.toStdString(), label.toStdString()));

            msim_rom *  comp_ptr = dynamic_cast< msim_rom *>(cpu.get_component(id.toStdString()));
            msim_rom_widget * widget = new msim_rom_widget{comp_ptr, loader, id};
            return{msim_cpu::get_instance().get_component(id.toStdString()), widget};
        }else if (id.contains("clock")){
            auto & cpu = msim_cpu::get_instance();
            cpu.add_component(std::make_unique<msim_clock>(id.toStdString(), label.toStdString()));

            msim_clock * comp_ptr =  dynamic_cast<msim_clock *>(cpu.get_component(id.toStdString()));
            msim_clock_widget * widget = new msim_clock_widget{comp_ptr, loader, id};
            return{comp_ptr, widget};

        }else if(id.contains("ar")){
            auto & cpu = msim_cpu::get_instance();
            cpu.add_component(std::make_unique<msim_ar>(id.toStdString(), label.toStdString()));

            msim_ar *  comp_ptr = dynamic_cast< msim_ar *>(cpu.get_component(id.toStdString()));
            msim_ar_widget * widget = new msim_ar_widget{comp_ptr, loader, id};
            return{msim_cpu::get_instance().get_component(id.toStdString()), widget};
        }
        else if(id.contains("cop")){
            auto & cpu = msim_cpu::get_instance();
            cpu.add_component(std::make_unique<msim_cop>(id.toStdString(), label.toStdString()));

            msim_cop *  comp_ptr = dynamic_cast< msim_cop *>(cpu.get_component(id.toStdString()));
            msim_cop_widget * widget = new msim_cop_widget{comp_ptr, loader, id};
            return{msim_cpu::get_instance().get_component(id.toStdString()), widget};
        }
        return {nullptr, nullptr};
    }

};

#endif // COMPONENTS_FACTORY_H
