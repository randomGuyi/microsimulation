#include "editor_controller.h"

#include <QRegularExpression>
#include <__ranges/views.h>

#include "ui/controller/msim_components_manager.h"
#include "../../core/parser/msim_parser.h"
using namespace gui::views;
editor_controller::editor_controller(QsciScintilla * editor, QObject * parent)
: QObject{parent}
, m_editor{editor}
    , m_scanner{new core::parser::msim_scanner{"", true}}
    , m_parser{new core::parser::msim_parser{m_scanner, nullptr}}
    , m_on_rom_subscribed{false} {

    m_editor->markerDefine(QsciScintilla::Rectangle, 1); // define marker id 1
    m_editor->setMarkerBackgroundColor(QColor("#FFF59D"), 1); // light yellow
    m_editor->setMarkerForegroundColor(QColor("#000000"), 1); // optional text color for marker

}
void editor_controller::highlight_line(int line)
{
    m_editor->markerDeleteAll();
    m_editor->markerAdd(line, 1);
}

void editor_controller::parse_and_highlight(){
    m_text = m_editor->text();
    m_scanner->reset(m_text.toStdString());

    core::components::msim_rom * rom_inst = fac::msim_components_manager::get_instance().get_rom();
    m_parser->set_scanner_inst(m_scanner);
    m_parser->set_rom_inst(rom_inst);

    if (rom_inst != nullptr && !m_on_rom_subscribed) {
        rom_inst->subscibe([this](int new_line){
            QMetaObject::invokeMethod(this, [this, new_line]{
                on_rom_changed(new_line);
            });
        }) ;
        m_on_rom_subscribed = true;
    }

    try{
        m_parser->parse();
        m_editor->clearAnnotations();
        update_markers(m_parser->get_errors());
    }catch(const std::runtime_error & err){
        m_editor->annotate(1, err.what(), QsciScintilla::AnnotationBoxed);
    }
}

void editor_controller::on_rom_changed(int line_number) {
    QString pattern = QString(R"(^[[:space:]]*(?:#\d+/%1|%1)[[:space:]]*;)").arg(line_number);

    QRegularExpression re(pattern, QRegularExpression::MultilineOption);

    // 2. Search the editor text
    QString text = m_editor->text();
    QRegularExpressionMatch match = re.match(text);

    if (match.hasMatch()) {
        int charPosition = match.capturedStart();

        int visualLine = m_editor->SendScintilla(QsciScintilla::SCI_LINEFROMPOSITION, charPosition) +1;

        // 4. Update the UI
        m_editor->markerDeleteAll();
        m_editor->markerAdd(visualLine, 1);
    }
}


void editor_controller::update_markers(const std::vector<core::parser::parser_error> & errors){
    m_editor->markerDeleteAll();
    m_editor->clearAnnotations();
    for(auto & err : errors){
        m_editor->markerAdd(err.line -1, 1);
        m_editor->annotate(err.line -1, QString::fromStdString(err.message), 1);
    }
}
