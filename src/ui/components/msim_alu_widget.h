#ifndef MSIM_ALU_WIDGET_H
#define MSIM_ALU_WIDGET_H

#include "msim_component_svg_widget.h"

#include "../../shared/svg_loader.h"
#include "../../core/components/msim_alu.h"



class msim_alu_widget : public msim_component_svg_widget{
    Q_OBJECT
public:
    msim_alu_widget(msim_alu * alu,
                    shared::svg_loader * loader,
                    QString const & element_id,
                    QWidget * parent = nullptr);

    virtual void update_display() override;

    virtual void show_tooltip() override;
    virtual void hide_tooltip() override;
protected:
    msim_alu * m_alu;


};

#endif // MSIM_ALU_WIDGET_H
