#ifndef LINE_FACTORY_H
#define LINE_FACTORY_H

#include "core/components/msim_line.h"
#include "ui/components/msim_line_widget.h"

class line_factory
{
public:
    static std::pair<msim_line * , msim_line_widget *>
    create(const QString & id, const QString & label, shared::svg_loader * loader){
        msim_line  * line = new msim_line{id.toStdString(), label.toStdString()};
        msim_line_widget * wdgt = new msim_line_widget{line, loader, id};
        return {line, wdgt};
    }
};

#endif // LINE_FACTORY_H
