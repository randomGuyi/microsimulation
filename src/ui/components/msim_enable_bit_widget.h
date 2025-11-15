#ifndef MSIM_ENABLE_BIT_WIDGET_H
#define MSIM_ENABLE_BIT_WIDGET_H

#include "core/components/msim_enable_bit.h"
#include "ui/components/msim_bit_widget.h"
class msim_enable_bit_widget : public msim_bit_widget{
public:
    msim_enable_bit_widget(msim_enable_bit * bit,
                           shared::svg_loader * loader,
                    QString const & element_id,
                    QWidget * parent = nullptr);

    virtual void update_display() override;

    virtual void show_tooltip() override;
    virtual void hide_tooltip() override;
protected:
    msim_enable_bit * m_bit;
};

#endif // MSIM_ENABLE_BIT_WIDGET_H
