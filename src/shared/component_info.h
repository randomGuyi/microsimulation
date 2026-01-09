/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MICROSIMULATION_COMPONENT_INFO_H
#define MICROSIMULATION_COMPONENT_INFO_H
#include <QtGui/qpixmap.h>
namespace shared {
    struct component_info {
        QString id;
        QString label;
        QPixmap icon;

    };
}
#endif //MICROSIMULATION_COMPONENT_INFO_H