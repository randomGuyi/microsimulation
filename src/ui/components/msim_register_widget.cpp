#include "msim_register_widget.h"
#include "ui/styles.h"

#include <QHBoxLayout>
#include <QToolTip>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QGraphicsProxyWidget>

msim_register_widget::msim_register_widget(msim_register * reg, QString const & label, QWidget * parent)
    :msim_component_widget{reg, parent}
    , m_register{reg}
    , m_format{int_fmt}
{
    setStyleSheet(REGISTER_STYLE_SHEET);
    /* label */
    m_label_widget = new QLabel{label, this};
    m_label_widget->setStyleSheet( REGISTER_LABLE_STYLE);
    m_label_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    /* value label*/
    m_value_widget = new QLabel("0", this);
    m_value_widget->setAlignment(Qt::AlignCenter);
    m_value_widget->setStyleSheet(REGISTER_VALUE_STYLE);
    m_value_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    /* layout */
    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_label_widget, 1); /* stretch factor */
    layout->addWidget(m_value_widget, 2);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    layout->setSizeConstraint(QLayout::SetMaximumSize);

    /* tooltip */
    QFont tooltip_font{};
    tooltip_font.setPointSize(14);
    QToolTip::setFont(tooltip_font);

    m_register->subscibe([this](int new_value){
        QMetaObject::invokeMethod(this, [this, new_value]{
            on_core_value_canged(new_value);
        });
    });

    update_display();
}

void msim_register_widget::on_core_value_canged(int new_value){
    update_display();
}


QGraphicsProxyWidget * msim_register_widget::attach_to_target(drop_target * target){
    auto proxy = msim_component_widget::attach_to_target(target);
    if(this->width() < this->height()){
        proxy->setRotation(90);
        this->setFixedSize({target->boundingRect().size().toSize().height(), target->boundingRect().size().toSize().width()});
        proxy->setPos(target->boundingRect().topRight());
    }
    return proxy;
}
void msim_register_widget::mousePressEvent(QMouseEvent * event) {
    if(event->button() == Qt::LeftButton){
        /* hop to the next format */
        m_format = static_cast<display_format>((m_format + 1) % 4);
        update_display();
        emit clicked();
    }
    QPushButton::mousePressEvent(event);
}

void msim_register_widget::update_display() {
    QString valueStr;
    switch (m_format) {
        case int_fmt: valueStr = QString::number(m_register->getValue()); break;
        case ascii_fmt: valueStr = to_ascii(m_register->getValue()); break;
        case hex_fmt: valueStr = to_hex(m_register->getValue()); break;
        case bin_fmt: valueStr = to_bin(m_register->getValue()); break;
    }
    m_value_widget->setText(valueStr);

}

void msim_register_widget::show_tooltip() {
    QString tooltip = QString{"Dec: %1\nHex: %2\nBin: %3\nAscii: %4\n \n (click to change)"}
                          .arg(m_register->getValue())
                          .arg(QString::number(m_register->getValue(), 16).toUpper())
                          .arg(QString::number(m_register->getValue(), 2))
                          .arg((m_register->getValue() >= 32 && m_register->getValue() <= 126 ) ? QChar(m_register->getValue()) : '.');
    QToolTip::showText(cursor().pos(), tooltip, this, QRect(), 500000); /* show tooltip for 5 sec */
}

void msim_register_widget::hide_tooltip() {
    QToolTip::hideText();
}

QString msim_register_widget::to_ascii(int val) const{
    if(val >= 32 && val <+ 126){
        return QChar{val};
    }
    return ".";
}

QString msim_register_widget::to_hex(int val) const{
    return QString("0x%1").arg(val, 2, 16, QChar('0'));
}

QString msim_register_widget::to_bin(int val) const{
    return QString("0x%1").arg(val, 8, 2, QChar('0'));
}
