#ifndef EDITOR_CONTROLLER_H
#define EDITOR_CONTROLLER_H

#include "../../core/parser/msim_parser.h"
#include "../../core/parser/msim_scanner.h"

#include <QObject>
#include <QsciScintilla.h>

#include "core/components/msim_cpu.h"

namespace gui::views {
    class editor_controller : public QObject{
        Q_OBJECT
    public:
        explicit editor_controller(QsciScintilla * editor, QObject * parent = nullptr);

        void highlight_line(int line);

        bool parse_and_highlight();

        int line_to_editor_line(int line_number) const;

        void on_cpu_changed(std::pair<core::sim::cpu_event_type, int> event);

    private:
        QsciScintilla * m_editor;
        core::parser::msim_scanner * m_scanner;
        core::parser::msim_parser * m_parser;
        bool m_on_rom_subscribed;
        QString m_text;
        //components_manager * m_components_manager;

        void update_markers(const std::vector<core::parser::parser_error> & errors);

    };
}
#endif // EDITOR_CONTROLLER_H
