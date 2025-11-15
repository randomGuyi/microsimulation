//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_DETAILS_VIEW_H
#define MICROSIMULATION_DETAILS_VIEW_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class details_view;
}

QT_END_NAMESPACE

class details_view : public QWidget {
    Q_OBJECT

public:
    explicit details_view(QWidget *parent = nullptr);

    ~details_view() override;
public slots:
   /* void show_compile_errors(std::vector<parser_error> const & errors);
    void show_compile_details(QString const & msg);
    void show_ram_details(ram * inst);
    void show_rom_details(rom * inst); */

private:
    Ui::details_view *ui;
    details_tabs * m_tabs;
};



#endif //MICROSIMULATION_DETAILS_VIEW_H