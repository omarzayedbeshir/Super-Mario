#ifndef CASTLE_H
#define CASTLE_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class Castle : public QGraphicsPixmapItem {
public:
    Castle(qreal x, qreal y, QGraphicsItem* parent = nullptr);
};

#endif // CASTLE_H
