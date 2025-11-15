#ifndef MSIM_REGISTER_WIDGET_H
#define MSIM_REGISTER_WIDGET_H

#include "core/components/msim_register.h"
#include "msim_component_widget.h"

#include <QLabel>


class msim_register_widget : public msim_component_widget{
    Q_OBJECT
public:
    enum display_format {int_fmt, ascii_fmt, hex_fmt, bin_fmt};
    explicit msim_register_widget(msim_register * reg, QString const & label,   QWidget * parent = nullptr);

    virtual void update_display() override;
    QGraphicsProxyWidget * attach_to_target(drop_target * target) override;

protected:
    void mousePressEvent(QMouseEvent * event) override;
    virtual void show_tooltip() override;
    virtual void hide_tooltip() override;
private:
    msim_register * m_register;

    QString to_ascii(int val) const;
    QString to_hex(int val) const;
    QString to_bin(int val) const;


    QLabel * m_label_widget;
    QLabel * m_value_widget;

    display_format m_format;
    void on_core_value_canged(int new_value);
};

#endif // MSIM_REGISTER_WIDGET_H
