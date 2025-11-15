#include "line_factory.h"
#include "msim_components_manager.h"
#include "core/common/id_reader.h"
#include "ui/controller/components_factory.h"
#include "shared/svg_loader.h"
#include "ui/controller/connector_factory.h"
#include <QRegularExpression>

msim_components_manager::msim_components_manager()
    : m_clock_placed{false}
    , m_ar_placed{false}
    , m_op_placed{false}
    , m_mode_placed{false}
    , m_renderer{&shared::svg_loader::get_instance().get_renderer()}
    , m_loader{&shared::svg_loader::get_instance()}
{}

QStringList msim_components_manager::find_connectors(msim_component const * placed_component){
    QStringList result{};

    QString placed_id = QString::fromStdString(placed_component->id());
    placed_id.remove("comp_");
    if(! placed_id.contains(QRegularExpression{"(clock|register|Bus|cop)"})){
        return result;
    }
    /* pcb_[a-z]*_ID | pcb_ID_[a-z]*  */
    std::string regx {"(pcb_.*" + placed_id.toStdString()};
    regx.append("_?|pcb_" + placed_id.toStdString() + ".*)");

    /* hot connectors */
    QStringList cons_ids{id_reader::get_instance().get_results_for(regx)};

    for(auto & conn_id : cons_ids){
        /* find the components, which should be placed in order to connect them */
        QStringList connector_components = id_reader::get_instance().get_postfix_components_for_id(conn_id);
        auto conn_id_it = connector_components.begin();
        if((*conn_id_it) == placed_id){
            /* we know its alredy placed */
            ++conn_id_it;
        }

        if(m_placed_components.find((*conn_id_it)) == m_placed_components.end() &&
            m_placed_components.find("comp_" + (*conn_id_it)) == m_placed_components.end()){
            continue;
        }
        result.append(conn_id);
    }
    return result;
}

QStringList msim_components_manager::find_lines_for_connector(QString const & connector_id){

    QStringList connector_components = id_reader::get_instance().get_postfix_components_for_id(connector_id);
    /* looking for the line-id's*/
    std::string first_component = connector_components[0].toStdString() + "_";
    std::string secound_component = connector_components[1].toStdString();
    /* in */
    std::string line_regex{"lineIn_" + first_component + secound_component};
    QStringList lines{id_reader::get_instance().get_results_for(line_regex)};
    /* out */
    line_regex = "lineOut_" + first_component + secound_component;
    lines.append(id_reader::get_instance().get_results_for(line_regex));
    /* en */
    line_regex = "lineEn_" + first_component + secound_component;
    lines.append(id_reader::get_instance().get_results_for(line_regex));
    /* clock */
    if(m_clock_placed){
        line_regex = "line.*_clock_" + first_component + secound_component;
        lines.append(id_reader::get_instance().get_results_for(line_regex));
    }
    return lines;
}

void msim_components_manager::place_clock_lines(drop_target * target){
    for(auto con_id_it{m_placed_connectors.keyBegin()};
         con_id_it != m_placed_connectors.keyEnd(); ++ con_id_it ){
        QStringList conn_pfc = id_reader::get_instance().get_postfix_components_for_id(*con_id_it);
        QString id{};
        if((*con_id_it).contains("ZBus")){
            id = "lineExecute_clock_";
        }else{
            id = "lineFetch_clock_";
        }
        id.append(conn_pfc[0]);
        id.append("_");
        id.append(conn_pfc[1]);

        auto [line, widget] = line_factory::create(id, id, m_loader);
        widget->attach_to_target(target);

    }
}

void msim_components_manager::place_component(drop_target * target, const QString & id, const QString & label){
    auto [comp, widget] = components_factory::create(id, label, m_loader);
    if(!comp || !widget) return;

    widget->attach_to_target(target);

    m_placed_components[id] = comp;
    if(! m_clock_placed && id.contains("clock")){
        place_clock_lines(target);
        m_clock_placed = true;
    }

    if((! m_ar_placed) && (id == "comp_ar")){
        place_ar(target);
        m_ar_placed = true;
    }
    if((! m_op_placed) && (id.contains("registerOp"))){
        place_op(target);
        m_op_placed = true;
    }
    if((! m_mode_placed) && (id.contains("registerMode"))){
        place_mode(target);
        m_mode_placed = true;
    }

    place_connector(target, comp);
    place_lines(target, comp);
}

void msim_components_manager::place_ar( drop_target * target){
        QStringList bit_reg{"maskBit", "cnBit", "modeBit"};
        QStringList line_reg{"lineMaskBit", "lineCnBit", "lineModeBit" };
        QStringList shared_lines{"lineMask_ar", "lineCn_ar", "lineMode_ar"};
        msim_line_widget * ar_line = line_factory::create("line_ar", "line_ar", m_loader).second;
        ar_line->attach_to_target(target);

        for(int i = 0; i < 3; ++i){

            /* place all components for the ar */
            QStringList mask_bits = id_reader::get_instance().get_results_for(bit_reg[i].toStdString()+ "[0-3]_ar");
            QStringList mask_lines = id_reader::get_instance().get_results_for(line_reg[i].toStdString() +"[0-3]_ar");

            if(mask_bits.size() != mask_lines.size()) break;

            mask_bits.sort();
            mask_lines.sort();
            msim_line_widget * mask_line = line_factory::create(shared_lines[i], shared_lines[i], m_loader).second;
            mask_line->attach_to_target(target);


            for(int i = 0; i < mask_bits.size(); ++i){
                auto [bit, bit_wdgt]  = connector_factory::create_enable_bit(mask_bits[i], mask_bits[i], m_loader);
                auto [line, line_wdgt] = line_factory::create(mask_lines[i], mask_lines[i], m_loader);
                bit_wdgt->add_line(line_wdgt);
                bit_wdgt->add_line(mask_line);
                bit_wdgt->add_line(ar_line);
                bit_wdgt->attach_to_target(target);
                line_wdgt->attach_to_target(target);
            }
        }
    }

void msim_components_manager::place_op(drop_target * target){
    QStringList bit_ids = id_reader::get_instance().get_results_for("opBit[0-4]_registerOp");
    QStringList line_ids = id_reader::get_instance().get_results_for("lineBit[0-4]_registerOp");
    bit_ids.sort(); line_ids.sort();

    msim_line_widget * main_line = line_factory::create("line_registerOp", "line_registerOp", m_loader).second;
    main_line->attach_to_target(target);

    for(int i = 0; i < bit_ids.size(); ++i){
        msim_bit_widget * bit = connector_factory::create_enable_bit(bit_ids[i], bit_ids[i], m_loader).second;
        msim_line_widget * line = line_factory::create(line_ids[i], line_ids[i], m_loader).second;
        bit->attach_to_target(target);
        line->attach_to_target(target);

        bit->add_line(line);
        bit->add_line(main_line);
        //qDebug() << "bit : " << bit_ids[i] << " line : " << line_ids[i];
    }

}

void msim_components_manager::place_mode(drop_target * target){
    msim_line_widget * main_line = line_factory::create("line_registerMode", "line_registerMode", m_loader).second;
    msim_line_widget * line_bit_0 = line_factory::create("lineModeBit0_registerMode", "lineModeBit0_registerMode", m_loader).second;
    msim_line_widget * line_bit_1 = line_factory::create("lineModeBit1_registerMode", "lineModeBit1_registerMode", m_loader).second;

    msim_bit_widget * mode_bit_0 = connector_factory::create_enable_bit("modeBit0_registerMode", "modeBit0_registerMode", m_loader).second;
    msim_bit_widget * mode_bit_1 = connector_factory::create_enable_bit("modeBit1_registerMode", "modeBit1_registerMode", m_loader).second;

    main_line->attach_to_target(target);
    line_bit_0->attach_to_target(target);
    line_bit_1->attach_to_target(target);

    mode_bit_0->attach_to_target(target);
    mode_bit_0->add_line(line_bit_0);

    mode_bit_1->attach_to_target(target);
    mode_bit_1->add_line(line_bit_1);
}

void msim_components_manager::place_lines(drop_target * target , msim_component const * placed_component){

    //qDebug() << "########################   place_lines called   ################################";
    QString dropped_id{QString::fromStdString(placed_component->id())};
    dropped_id.remove("comp_");


    QStringList lines_no_con = id_reader::get_instance().get_results_for("^line_.*");

    for(auto & id : lines_no_con){

        /* these lines are all placed in seperate functions */
        if(id == "line_ar" || id.contains("clock") || id == "line_registerOp" || id == "line_registerMode") continue;
        if(m_placed_lines.find(id) != m_placed_lines.end()) continue;
        bool will_fit{true};

        QStringList comps = id_reader::get_instance().get_postfix_components_for_id(id);

        for(auto  & c : comps){
            auto wdgt = std::find_if(m_placed_components.begin(), m_placed_components.end(),
                                     [&c](auto & pair){
                QString id = QString::fromStdString(pair->id());
                id.remove("comp_");
                return c == id;
            });

            if(wdgt == m_placed_components.end()){
                will_fit = false;
                break;
            }
        }
        if(will_fit){
            auto new_line =  line_factory::create(id, id, m_loader).second;
            new_line->attach_to_target(target);
            m_placed_lines.insert(id, new_line);
        }
    }





}
void msim_components_manager::place_connector(drop_target * target, msim_component const * placed_component){

    QString placed_id = QString::fromStdString(placed_component->id());
    if(! placed_id.contains(QRegularExpression{"(clock|register|Bus|cop)"})){
        return;
    }
    QStringList connectors{find_connectors(placed_component)};
    if(connectors.empty()) return;

    for(auto & connector_id : connectors){
        QStringList lines {find_lines_for_connector(connector_id)};

        auto [connector, connector_widget]  = connector_factory::create(connector_id, connector_id, m_loader);
        if(! connector || !connector_widget) continue;
        connector_widget->attach_to_target(target);

        for(auto &line_id : lines){
            auto [line, line_widget] = line_factory::create(line_id, line_id, m_loader);
            connector_widget->add_line(line_widget);
            line_widget->attach_to_target(target);

            if(line_id.contains("En")){
                QString enable_bit_id = connector_id;
                enable_bit_id.remove("pcb");
                enable_bit_id.prepend("enBit");
                auto [bit, bit_widget] = connector_factory::create_enable_bit(enable_bit_id, enable_bit_id, m_loader);
                bit_widget->add_line(line_widget);
                bit_widget->attach_to_target(target);
            }
        }


        m_placed_connectors[connector_id] = connector;
    }
}

msim_rom * msim_components_manager::get_rom(){
    QMap<QString, msim_component * >::Iterator it = m_placed_components.find("comp_rom");
    if(it == m_placed_components.end()){
        return nullptr;
    }
    return dynamic_cast<msim_rom *>(it.value());
}








