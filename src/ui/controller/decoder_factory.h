//
// Created by Arthur on 19.11.25.
//

#ifndef MICROSIMULATION_DECODER_FACTORY_H
#define MICROSIMULATION_DECODER_FACTORY_H
#include <QtCore/qstring.h>

#include "core/components/msim_decoder.h"
#include "ui/components/msim_decoder_widget.h"
#include "ui/components/msim_line_widget.h"

namespace fac {
    class decoder_factory {
    public:
        static std::pair<core::components::msim_decoder *, gui::components::msim_decoder_widget *>
        create(const QString & id, const QString & label, shared::svg_loader * loader) {
            using namespace core::components;
            using namespace gui::components;
            msim_decoder * decoder = new msim_decoder{id.toStdString(), label.toStdString()};
            msim_decoder_widget * decoder_widget = new msim_decoder_widget (decoder, loader, id);
            return {decoder, decoder_widget};
        }
    };
}

#endif //MICROSIMULATION_DECODER_FACTORY_H