#ifndef MSIM_ALU_WIDGET_H
#define MSIM_ALU_WIDGET_H

#include <QLabel>

#include "msim_component_svg_widget.h"

#include "../../shared/svg_loader.h"
#include "../../core/components/msim_alu.h"
#include "ui/highlight_label.h"


class msim_alu_widget : public msim_component_svg_widget{
    Q_OBJECT
public:
    msim_alu_widget(msim_alu * alu,
                    shared::svg_loader * loader,
                    QString const & element_id,
                    QWidget * parent = nullptr);

    void on_core_value_changed(std::pair<mux_selection, int> new_result);

    QString getOperation();

    void update_display() override;

    void show_tooltip() override;
    void hide_tooltip() override;
protected:
    msim_alu * m_alu;

    QLabel * m_operation_label;
    HighlightLabel  m_lu_label;
    HighlightLabel  m_au_label;
    HighlightLabel m_mux_label;
};

#endif // MSIM_ALU_WIDGET_H
