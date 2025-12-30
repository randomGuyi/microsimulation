#include "editor_controller.h"

#include <__ranges/views.h>

#include "ui/controller/msim_components_manager.h"
#include "../../core/parser/msim_parser.h"
using namespace gui::views;
editor_controller::editor_controller(QsciScintilla * editor, QObject * parent)
: QObject{parent}
, m_editor{editor}
    , m_scanner{new core::parser::msim_scanner{"", true}}
    , m_parser{new core::parser::msim_parser{m_scanner, nullptr}}
{}

void editor_controller::parse_and_highlight(){
    QString text = m_editor->text();
    m_scanner->reset(text.toStdString());

    core::components::msim_rom * rom_inst = fac::msim_components_manager::get_instance().get_rom();
    m_parser->set_scanner_inst(m_scanner);
    m_parser->set_rom_inst(rom_inst);

    try{
        m_parser->parse();
        m_editor->clearAnnotations();
        update_markers(m_parser->get_errors());
    }catch(const std::runtime_error & err){
        m_editor->annotate(1, err.what(), QsciScintilla::AnnotationBoxed);
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
