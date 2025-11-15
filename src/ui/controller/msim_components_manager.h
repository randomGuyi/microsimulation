#ifndef MSIM_COMPONENTS_MANAGER_H
#define MSIM_COMPONENTS_MANAGER_H

#include "core/components/msim_component.h"
#include "core/components/msim_rom.h"

#include "shared/svg_loader.h"
#include "ui/components/msim_line_widget.h"
#include "ui/drag_and_drop/drop_target.h"

#include <QGraphicsScene>
class msim_components_manager
{
public:

    static msim_components_manager & get_instance(){
        static msim_components_manager instance{};
        return instance;
    }

    void place_component(drop_target * target, const QString & id, const QString & label);
    msim_rom * get_rom();

private:
    void place_clock_lines(drop_target * target);
    msim_components_manager();
    ~msim_components_manager() = default;
    msim_components_manager(const msim_components_manager & other) = delete;
    msim_components_manager & operator=(const msim_components_manager & other) = delete;

    void place_connector(drop_target * target , msim_component const * placed_component);
    void place_lines(drop_target * target , msim_component const * placed_component);

    void place_ar( drop_target * target);
    void place_op(drop_target * target);
    void place_mode(drop_target * target);


    QStringList find_connectors(msim_component const * placed_component);
    QStringList find_lines_for_connector(QString const & connector_id);

    /* using bits to simplify line and bit placement for specific components */
    bool m_clock_placed;
    bool m_ar_placed;
    bool m_op_placed;
    bool m_mode_placed;

    QSvgRenderer * m_renderer;
    shared::svg_loader * m_loader;
    QMap<QString, msim_component *> m_placed_components;
    QMap<QString, msim_component *> m_placed_connectors;
    QMap<QString, msim_line_widget *> m_placed_lines;

};

#endif // MSIM_COMPONENTS_MANAGER_H
