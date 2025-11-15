#include "msim_bit_widget.h"
#include "ui/styles.h"

msim_bit_widget::msim_bit_widget(msim_bit * bit, shared::svg_loader * loader, QString const & element_id, QWidget * parent)
:msim_component_svg_widget{loader, element_id, bit, parent}
, m_bit{bit}
, m_current_value{bit->value()}
{
    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet(BIT_DISPLAY_LABEL);
    m_label->setText(m_current_value ? "1" : "0");
    m_label->setAttribute(Qt::WA_TransparentForMouseEvents);

    m_label->setGeometry(this->rect());


    m_bit->subscibe([this](bool new_val){
        QMetaObject::invokeMethod(this, [this, new_val]{
            on_core_value_changed(new_val);
        });
    }) ;

}

void msim_bit_widget::resizeEvent(QResizeEvent* event) {
    msim_component_svg_widget::resizeEvent(event);
    if (m_label) {
        m_label->setGeometry(this->rect());  // always cover widget
    }
}

void msim_bit_widget::add_line(msim_line_widget * line){
    m_lines.insert(line->id(), line);
}

void msim_bit_widget::remove_line(QString id){
    m_lines.remove(id);
}

void msim_bit_widget::remove_line(msim_line_widget * line){
    m_lines.remove(line->id());
}


void msim_bit_widget::on_core_value_changed(bool value){
    m_current_value = value;
    m_label->setText(value ? "1" : "0");
    update_display();
}

void msim_bit_widget::mousePressEvent(QMouseEvent * event){
    QPushButton::mousePressEvent(event);

    bool new_value = !m_current_value;
    m_bit->set_value(new_value);

    qDebug() << "UI clicked in msim_bit_widget";
}

