#ifndef EDITOR_CONTROLLER_H
#define EDITOR_CONTROLLER_H

#include "../../core/parser/msim_parser.h"
#include "../../core/parser/msim_scanner.h"

#include <QObject>
#include <QsciScintilla.h>

namespace gui::views {
    class editor_controller : public QObject{
        Q_OBJECT
    public:
        explicit editor_controller(QsciScintilla * editor, QObject * parent = nullptr);
        void parse_and_highlight();

    private:
        QsciScintilla * m_editor;
        core::parser::msim_scanner * m_scanner;
        core::parser::msim_parser * m_parser;
        //components_manager * m_components_manager;

        void update_markers(const std::vector<core::parser::parser_error> & errors);

    };
}
#endif // EDITOR_CONTROLLER_H
