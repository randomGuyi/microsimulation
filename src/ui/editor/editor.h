//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_EDITOR_H
#define MICROSIMULATION_EDITOR_H

#include <QWidget>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercustom.h>
#include "../controller/editor_controller.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class editor;
}

QT_END_NAMESPACE

class editor : public QWidget {
    Q_OBJECT

public:
    explicit editor(QWidget *parent = nullptr);

    ~editor() override;
private slots:
    void run_parser();

private:
    Ui::editor *ui;
    QsciScintilla * m_editor;
    QsciLexerCustom* m_lexer;
    editor_controller * m_controller;

};


#endif //MICROSIMULATION_EDITOR_H