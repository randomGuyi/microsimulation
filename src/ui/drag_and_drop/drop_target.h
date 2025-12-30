//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_DROP_TARGET_H
#define MICROSIMULATION_DROP_TARGET_H

#include <QGraphicsSvgItem>
#include <QtCore/qtmetamacros.h>

namespace fac {
    class msim_components_manager;
}

namespace gui {
    class drop_target : public QGraphicsSvgItem{
        Q_OBJECT

    public:
        explicit drop_target(const QString & element_id);
        ~drop_target();
        void place_component(const QString &id, const QString &label);

    protected:
        void dragEnterEvent (QGraphicsSceneDragDropEvent * event) override;
        void dragLeaveEvent (QGraphicsSceneDragDropEvent * event) override;
        void dropEvent (QGraphicsSceneDragDropEvent * event) override;
        void hoverEnterEvent (QGraphicsSceneHoverEvent * event) override;
        void hoverLeaveEvent (QGraphicsSceneHoverEvent * event) override;

    private:
        QString clean_element_id() const;
    };
}


#endif //MICROSIMULATION_DROP_TARGET_H