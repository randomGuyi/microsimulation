#include "line_factory.h"
#include "msim_components_manager.h"
#include "core/common/id_reader.h"
#include "ui/controller/components_factory.h"
#include "shared/svg_loader.h"
#include "ui/controller/connector_factory.h"
#include <QRegularExpression>
#include <ranges>

#include "decoder_factory.h"
#include "shared/svg_loader.h"
#include "ui/views/simulator_area.h"

using namespace core::components;
using namespace gui::components;

namespace fac {

msim_components_manager::msim_components_manager()
    : m_clock_placed{false}
    , m_ar_placed{false}
    , m_op_placed{false}
    , m_mode_placed{false}
    , m_renderer{&shared::svg_loader::get_instance().get_renderer()}
    , m_loader{&shared::svg_loader::get_instance()}
{}

std::vector<std::string> msim_components_manager::get_placeable_lines_by_regex(std::string const & regex) {
    /* lines from and to component */
    QStringList lines = shared::id_reader::get_instance().get_results_for(regex);
    return placeable_ids_for(lines);
}

std::vector<std::string> msim_components_manager::placeable_line_ids(QString id){
    return get_placeable_lines_by_regex("line_.*" + id.remove("comp_").toStdString() + ".*");
}

std::vector<std::string> msim_components_manager::placeable_connector_line_ids(QString id){
    return get_placeable_lines_by_regex("line(Fetch|Execute|Out|In|En).*" + id.remove("pcb_").toStdString() + ".*");
}

std::vector<std::string> msim_components_manager::placeable_ids_for(QStringList const & component_ids) {
    std::vector<std::string> placeable_ids;
    auto to_comp = [](const QString & q) {
        /* register do not have a prefix */
        if (q.contains("register")) return q;
        return "comp_" + q;
    };
    /* filter components which are already placeable */
    for (const auto & component_id : component_ids) {
        /* all components required by the component*/
        QStringList necessary_components =
                shared::id_reader::get_instance().get_postfix_components_for_id(component_id);
        const bool all_present = std::all_of(
            necessary_components.begin(), necessary_components.end(),
            [this, to_comp](const QString &qs) {
                if (qs.contains("registerA")) {
                    return m_placed_components
                    .keys()
                    .filter(QRegularExpression("register.*"))
                    .length() > 0;
                }
                return m_placed_components.contains(to_comp(qs)) ? true : qs == "Dec";
            });
        if(all_present &! necessary_components.empty()) {
            placeable_ids.push_back(component_id.toStdString());
        }
    }
    return placeable_ids;
}

std::vector<std::string> msim_components_manager::placeable_connector_ids(QString id) {
    QStringList conns =
            shared::id_reader::get_instance().get_results_for("pcb.*" + id.remove("comp_").toStdString() + ".*");
    return placeable_ids_for(conns);
}

std::vector<std::string> msim_components_manager::placeable_bit_ids(QString id) {
    /*modeBit1_registerMode*/
    if (id.contains(QRegularExpression("register[0-3]"))) {
        id = "registerA";
    }
    QStringList const bits =
            shared::id_reader::get_instance().get_results_for(".*(B|b)it.*" + id.remove("comp_").toStdString() + ".*");
    return placeable_ids_for(bits);
}

std::pair<msim_connector*, msim_connector_widget* > msim_components_manager::get_or_create_connector(QString conn_id) {
    const auto connIt = m_placed_connectors.find(conn_id);
    if(connIt == m_placed_connectors.end()) {
        auto [connector, connector_widget]  =
            connector_factory::create(conn_id, conn_id, m_loader);
        m_placed_connectors[conn_id] = {connector, connector_widget};
    }
    return m_placed_connectors[conn_id];
}

std::pair<msim_decoder *, msim_decoder_widget *> msim_components_manager::get_or_create_decoder(QString decoder_id) {
    const auto dec_it= m_placed_decoders.find(decoder_id);
    if(dec_it == m_placed_decoders.end()) {
        auto [decoder, decoder_widget]  =
            decoder_factory::create(decoder_id, decoder_id, m_loader);
        m_placed_decoders[decoder_id] = {decoder, decoder_widget};
    }
    return m_placed_decoders[decoder_id];
}
std::pair<msim_component*, msim_component_widget *>
msim_components_manager::get_or_create_component(QString const & comp_id, QString const & label) {
    const auto comp_it= m_placed_components.find(comp_id);
    if(comp_it == m_placed_components.end()) {
        auto [comp, comp_widget]  =
            components_factory::create(comp_id, label, m_loader);

        m_placed_components[comp_id] = {comp, comp_widget};
    }
    return m_placed_components [comp_id];
}

std::pair<msim_bit *, msim_bit_widget *> msim_components_manager::get_or_create_bit(QString bit_id) {
   const auto bit_it = m_placed_bits.find(bit_id);
    if (bit_it == m_placed_bits.end()) {
        auto [bit, bit_widget]  =
            connector_factory::create_enable_bit(bit_id, bit_id, m_loader);
        m_placed_bits[bit_id] = {bit, bit_widget};
    }
    return m_placed_bits[bit_id];
}

std::pair<msim_line *, msim_line_widget *> msim_components_manager::get_or_create_line(QString const & line_id) {
    const auto line_it = m_placed_lines.find(line_id);
    if (line_it == m_placed_lines.end()) {
        auto [line, line_widget]  =
            line_factory::create(line_id,line_id , m_loader);
        m_placed_lines[line_id] = {line, line_widget};
    }
    return m_placed_lines[line_id];
}

QString msim_components_manager::get_lines_for_bit(QString bit_id) {
    QString replace = "line_";
    if (bit_id.contains(QRegularExpression("([Mm]ar|[Mm]dr)"))) {
        replace = "lineEn_";
    }
    else if (bit_id.contains("registerA")) {
        QString match = replace + bit_id;
        return match;
    }

    if (bit_id.startsWith("mode")) {
        /* there are 2 modes, ram read/write/nothing or ar (jmp, ...) */
        if (bit_id.endsWith("ar")) {
            replace = "lineMode_";
        }
    }
    QString result =  bit_id.replace(QRegularExpression("^.*[Bb]it[^_]*_"), replace);
    return result;
}

QString msim_components_manager::get_decoder_id_4_bit(QString bit_id) {
    if (bit_id.endsWith("registerA")) {
        bit_id.replace(0, 6, "Dec");
        return bit_id;
    }
    return "";
}

void msim_components_manager::place_connectors(QString const & id, gui::drop_target * target) {
    for (auto const & conn_id: placeable_connector_ids(id)) {
        bool const is_new = m_placed_connectors.find(QString::fromStdString(conn_id)) == m_placed_connectors.end();
        auto [connector, connector_widget] = get_or_create_connector(QString::fromStdString(conn_id));
        if(! connector || !connector_widget) continue;
        if (is_new) {
            connector_widget->attach_to_target(target);
        }
        for (auto const & conn_line :  placeable_connector_line_ids(QString::fromStdString(conn_id))) {
            bool const is_new_line = m_placed_lines.find(QString::fromStdString(conn_line)) == m_placed_lines.end();
            auto [line, line_widget] = get_or_create_line(QString::fromStdString(conn_line));
            connector_widget->add_line(line_widget);
            if (is_new_line) {
                line_widget->attach_to_target(target);
            }
        }
    }
}

void msim_components_manager::place_lines(QString const & id, gui::drop_target * target) {
    for (auto const & line_id : placeable_line_ids(id)) {
        bool const is_new_line = m_placed_lines.find(QString::fromStdString(line_id)) == m_placed_lines.end();
        auto [line, line_widget] = get_or_create_line(QString::fromStdString(line_id));
        if (is_new_line) line_widget->attach_to_target(target);
    }
}

void msim_components_manager::place_bits(QString const & id, gui::drop_target * target) {
    for (auto const & bit_id : placeable_bit_ids(id)) {
        bool const is_new_bit = m_placed_bits.find(QString::fromStdString(bit_id)) == m_placed_bits.end();
        auto [bit, bit_widget] = get_or_create_bit(QString::fromStdString(bit_id));
        if (is_new_bit) {
            bit_widget->attach_to_target(target);
            QString line_for_bit = get_lines_for_bit(QString::fromStdString(bit->id()));
            if (!m_placed_lines.contains(line_for_bit)) {
                auto [line, line_widget] = get_or_create_line(line_for_bit);
                line_widget->attach_to_target(target);
                bit_widget->add_line(line_widget);
            }
            /* place decoders */
            QString decoder_id4Bit = get_decoder_id_4_bit(QString::fromStdString(bit->id()));
            if (! decoder_id4Bit.isEmpty()) {
                bool is_new_dec = m_placed_decoders.find(decoder_id4Bit) != m_placed_decoders.end();
                if (! is_new_dec) {
                    auto [decoder, decoder_widget] = get_or_create_decoder(decoder_id4Bit);
                    decoder_widget->attach_to_target(target);
                }
            }
        }
    }
}

void msim_components_manager::place_component(gui::drop_target *target, const QString &id, const QString &label) {
    auto [component, widget] = get_or_create_component(id, label);
    if(!component || !widget) return;
    widget->attach_to_target(target);

    /* place connectors */
    place_connectors(id, target);

    /* place all lines, which do not need connectors */
    place_lines(id, target);

    /* place bits */
    place_bits(id, target);

}

msim_rom * msim_components_manager::get_rom(){
    const auto it = m_placed_components.find("comp_rom");
    if(it == m_placed_components.end()){
        return nullptr;
    }
    return dynamic_cast<msim_rom *>(it.value().first);
}
};
