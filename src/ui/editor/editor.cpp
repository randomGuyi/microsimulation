//
// Created by Arthur on 15.11.25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_editor.h" resolved

#include "microcodelexer.h"
#include "ui_editor.h"

#include "../controller/editor_controller.h"
#include "editor.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QsciScintilla.h>


editor::editor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::editor)
    , m_editor{new QsciScintilla}
    , m_lexer{new MicrocodeLexer{this}}
{
    ui->setupUi(this);

    // Use this widget itself, not a temporary one
    QVBoxLayout * layout = new QVBoxLayout(this);
    /* editor menue */
    QFrame *topBar = new QFrame(this);
    topBar->setFrameShape(QFrame::StyledPanel);
    topBar->setFrameShadow(QFrame::Raised);

    QHBoxLayout *topLayout = new QHBoxLayout(topBar);

    QPushButton *compileBtn = new QPushButton("Compile", topBar);
    compileBtn->setFixedHeight(28);
    compileBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    topLayout->addWidget(compileBtn);
    topLayout->addStretch();
    layout->addWidget(topBar);

    /* editor */

    layout->addWidget(m_editor);
    setLayout(layout);
/*    this->setStyleSheet("background: green;"
                        "border: 3px solid green;");
*/
    m_editor->setMarginType(0, QsciScintilla::NumberMargin);
    m_editor->setMarginWidth(0, "0000"); // Platz für 4-stellige Zeilen

    m_lexer->setColor(Qt::black, MicrocodeLexer::Styles::Default);
    m_lexer->setColor(Qt::magenta, MicrocodeLexer::Styles::Number);
    m_lexer->setColor(Qt::blue, MicrocodeLexer::Styles::Register);
    m_lexer->setColor(Qt::green, MicrocodeLexer::Styles::Bus);
    m_lexer->setColor(Qt::cyan, MicrocodeLexer::Styles::Operation);
    m_lexer->setColor(Qt::darkMagenta, MicrocodeLexer::Styles::Car);
    m_lexer->setColor(Qt::gray, MicrocodeLexer::Styles::Comment);
    m_lexer->setColor(Qt::red, MicrocodeLexer::Styles::Error);

    m_editor->setLexer(m_lexer);

    QFont font("Courier", 12);
    m_editor->setFont(font);
    m_editor->setMarginsFont(font);

    m_controller = new editor_controller{m_editor, this};
    connect(compileBtn, &QPushButton::pressed, this, &editor::run_parser);
}

void editor::run_parser(){
    qDebug() << "run parser !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    m_controller->parse_and_highlight();
}

editor::~editor()
{
    delete ui;
}
