#include "components_picker_controller.h"
#include "core/common/id_reader.h"
#include "../../shared/constants.h"

#include "shared/svg_loader.h"

components_picker_controller::components_picker_controller() {}

QVector<component_info> components_picker_controller::get_all_components() const{
    QVector<component_info> result{get_registers()};
    result.append(get_components());
    return result;
}


QVector<component_info> components_picker_controller::get_registers() const{

    QVector<component_info> result{};

    auto ids_register = id_reader::get_instance().get_results_for("^dropTarget_register.*");
    for(auto const & id_reg : ids_register){
        QString label= id_reg;
        label.remove("dropTarget_register");

        if(label.size() == 1){
            /* its one of R0, R1, R2, R3 */
            label = "R " + label;
        }else{
            /* FLAGS, MDR, .... */
            label = label.toUpper();
        }

        QString id = id_reg;
        id.remove("dropTarget_");
        result.append(component_info{id, label, QPixmap{}});
    }
    return result;
}


QVector<component_info> components_picker_controller::get_components() const{
    QVector<component_info> result{};
    auto ids_components = id_reader::get_instance().get_results_for("^comp_.*");

    for(auto const & id_comp: ids_components){

        QString label= id_comp;
        label.remove("comp_");

        QPixmap icon = shared::svg_loader::get_instance()
                           .get_pixmap_for_id(id_comp, QSize{SVG_ICON_SIZE, SVG_ICON_SIZE});

        result.append(component_info{id_comp, label, icon});
    }
    return result;
}
