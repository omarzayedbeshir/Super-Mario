#include "pipe.h"
#include "mario.h"
#include <QPixmap>

pipe::pipe(qreal x, qreal y, QGraphicsItem* parent)
    : QObject(), QGraphicsItemGroup(parent)
{
    top_part = new QGraphicsPixmapItem(QPixmap(":/graphics/Mario Game Assets/PipeTop.png"));
    top_part->setPos(x, y);
    bottom_part = new QGraphicsPixmapItem(QPixmap(":/graphics/Mario Game Assets/PipeBottom.png"));
    bottom_part->setPos(x, y + top_part->boundingRect().height());

    addToGroup(top_part);
    addToGroup(bottom_part);

    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setZValue(1);
}

QGraphicsPixmapItem* pipe::getTopPart() const {
    return top_part;
}



QRectF pipe::boundingRect() const
{
    return bottom_part->sceneBoundingRect();
}
