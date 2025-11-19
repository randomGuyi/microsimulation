#ifndef MSIM_COMPONENTS_MANAGER_H
#define MSIM_COMPONENTS_MANAGER_H

#include "core/components/msim_component.h"
#include "core/components/msim_rom.h"

#include "shared/svg_loader.h"
#include "ui/components/msim_line_widget.h"
#include "ui/drag_and_drop/drop_target.h"

#include <QGraphicsScene>

#include "core/components/msim_bit.h"
#include "ui/components/msim_bit_widget.h"
#include "ui/components/msim_connector_widget.h"
#include "ui/components/msim_decoder_widget.h"

class msim_components_manager
{
public:

    static msim_components_manager & get_instance(){
        static msim_components_manager instance{};
        return instance;
    }

    QString get_decoder_id_4_bit(QString bit_id);

    void place_component(drop_target * target, const QString & id, const QString & label);
    msim_rom * get_rom();

private:

    std::vector<std::string> placeable_connector_ids(QString id);

    std::vector<std::string> placeable_bit_ids(QString id);

    std::pair<msim_connector *, msim_connector_widget *> get_or_create_connector(QString conn_id);

    std::pair<msim_decoder *, msim_decoder_widget *> get_or_create_decoder(QString decoder_id);

    std::pair<msim_component *, msim_component_widget *> get_or_create_component(QString const &comp_id, QString const &label);

    std::pair<msim_bit *, msim_bit_widget *> get_or_create_bit(QString bit_id);

    std::pair<msim_line *, msim_line_widget *> get_or_create_line(QString const &line_id);

    QString get_lines_for_bit(QString bit_id);

    std::vector<std::string> placeable_line_ids(QString id) ;

    std::vector<std::string> placeable_connector_line_ids(QString id);

    std::vector<std::string> placeable_ids_for(QStringList const &component_ids);

    void place_clock_lines(drop_target * target);

    std::vector<std::string> get_placealble_lines_by_regex(std::string const &regex);

    msim_components_manager();
    ~msim_components_manager() = default;
    msim_components_manager(const msim_components_manager & other) = delete;
    msim_components_manager & operator=(const msim_components_manager & other) = delete;


    /* using bits to simplify line and bit placement for specific components */
    bool m_clock_placed;
    bool m_ar_placed;
    bool m_op_placed;
    bool m_mode_placed;

    QSvgRenderer * m_renderer;
    shared::svg_loader * m_loader;
    QMap<QString, std::pair<msim_component*, msim_component_widget*>> m_placed_components;
    QMap<QString, std::pair<msim_connector*, msim_connector_widget *>> m_placed_connectors;
    QMap<QString, std::pair<msim_line * , msim_line_widget *>> m_placed_lines;
    QMap<QString, std::pair<msim_decoder* , msim_decoder_widget*>> m_placed_decoders;
    QMap<QString, std::pair<msim_bit * , msim_bit_widget*>> m_placed_bits;

};

#endif // MSIM_COMPONENTS_MANAGER_H
