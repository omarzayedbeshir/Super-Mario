#include "castle.h"

Castle::Castle(qreal x, qreal y, QGraphicsItem* parent) : QGraphicsPixmapItem(parent) {
    QPixmap pixmap(":/graphics/Mario Game Assets/Castle.png");
    setPixmap(pixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setScale(2.0);
    setPos(x, y);
    setZValue(1);
}


