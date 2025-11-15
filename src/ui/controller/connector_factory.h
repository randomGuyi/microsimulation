#ifndef CONNECTOR_FACTORY_H
#define CONNECTOR_FACTORY_H

#include "core/components/msim_connector.h"
#include "core/components/msim_enable_bit.h"

#include "ui/components/msim_connector_widget.h"
#include "ui/components/msim_enable_bit_widget.h"

#include <core/components/msim_cpu.h>

class connector_factory {
public:
    static std::pair<msim_connector *, msim_connector_widget *>
    create(const QString & id, const QString & label, shared::svg_loader * loader){
        std::unique_ptr<msim_connector>  con = std::make_unique<msim_connector>(id.toStdString(), label.toStdString());
        msim_connector_widget * cw = new msim_connector_widget{con.get(), loader, id};
        msim_cpu::get_instance().add_connector(std::move(con));
        return {msim_cpu::get_instance().get_connector(id.toStdString()), cw};
    }

    static std::pair<msim_enable_bit * , msim_enable_bit_widget *>
    create_enable_bit(QString const & id, QString const & label, shared::svg_loader * loader){
        std::unique_ptr<msim_enable_bit>  en_bit = std::make_unique<msim_enable_bit>(id.toStdString(), label.toStdString());
        msim_enable_bit_widget * en_bit_wdgt = new msim_enable_bit_widget{en_bit.get(), loader, id};
        msim_cpu::get_instance().add_enable_bit(std::move(en_bit));
        return {msim_cpu::get_instance().get_enable_bit(id.toStdString()), en_bit_wdgt};
    }
};

#endif // CONNECTOR_FACTORY_H
