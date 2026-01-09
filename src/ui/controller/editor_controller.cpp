/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#include "editor_controller.h"

#include <QRegularExpression>
#include <__ranges/views.h>

#include "ui/controller/msim_components_manager.h"
#include "../../core/parser/msim_parser.h"
#include "core/components/msim_cpu.h"
using namespace gui::views;
using namespace std::ranges::views;

editor_controller::editor_controller(QsciScintilla * editor, QObject * parent)
: QObject{parent}
, m_editor{editor}
    , m_scanner{new core::parser::msim_scanner{"", true}}
    , m_parser{new core::parser::msim_parser{m_scanner, nullptr}}
    , m_on_rom_subscribed{false} {

    m_editor->markerDefine(QsciScintilla::Rectangle, 1); // define marker id 1
    m_editor->setMarkerBackgroundColor(QColor("#FFF59D"), 1); // light yellow
    m_editor->setMarkerForegroundColor(QColor("#000000"), 1);

    core::sim::msim_cpu::get_instance().subscibe([this](std::pair<core::sim::cpu_event_type, int> event){
            QMetaObject::invokeMethod(this, [this, event]{
                on_cpu_changed(event);
            });
        }) ;
        m_on_rom_subscribed = true;
    }

void editor_controller::highlight_line(int line)
{
    m_editor->markerDeleteAll();
    m_editor->markerAdd(line, 1);
}

bool editor_controller::parse_and_highlight(){
    m_text = m_editor->text();
    m_scanner->reset(m_text.toStdString());

    core::components::msim_rom * rom_inst = fac::msim_components_manager::get_instance().get_rom();
    m_parser->set_scanner_inst(m_scanner);
    m_parser->set_rom_inst(rom_inst);


    try{
        m_parser->parse();
        m_editor->clearAnnotations();
        update_markers(m_parser->get_errors());
    }catch(const std::runtime_error & err){
        m_editor->annotate(1, err.what(), QsciScintilla::AnnotationBoxed);
        return false;
    }
    return true;
}

int editor_controller::line_to_editor_line(int line_number) const {
    QString pattern = QString(R"(^[[:space:]]*(?:#\d+/%1|%1)[[:space:]]*;)").arg(line_number);

    QRegularExpression re(pattern, QRegularExpression::MultilineOption);

    /* search in editor text for the correct segment */
    QString text = m_editor->text();
    QRegularExpressionMatch match = re.match(text);

    if (match.hasMatch()) {
        int charPosition = match.capturedStart();

        return  m_editor->SendScintilla(QsciScintilla::SCI_LINEFROMPOSITION, charPosition) +1;
    }
    return - 1;
}

void editor_controller::on_cpu_changed(std::pair<core::sim::cpu_event_type, int> event) {
    int visualLine = line_to_editor_line(event.second);
    switch (event.first) {
        case core::sim::cpu_event_type::NEXT_INSTRUCTION_LOADED: {
            if (visualLine == -1) return;
            /* mark line */
            m_editor->markerDeleteAll();
            m_editor->markerAdd(visualLine, 1);
            break;
        }
        case core::sim::cpu_event_type::ERROR_OCCURRED: {
            for (auto &err: core::sim::msim_cpu::get_instance().get_errors()) {
                visualLine = line_to_editor_line(event.second) - 1;
                visualLine  = visualLine < 0 ? 0  :  visualLine;
                m_editor->markerAdd(visualLine, 1);
                m_editor->annotate(visualLine , QString::fromStdString(err.message), 1);
            }
           break;
        }
            default: break;
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
