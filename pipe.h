#ifndef PIPE_H
#define PIPE_H

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QObject>


class Mario;

class pipe : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    pipe(qreal x, qreal y, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    QGraphicsPixmapItem* getTopPart() const;
    QGraphicsPixmapItem* getBottomPart() const;


private:
    QGraphicsPixmapItem* top_part;
    QGraphicsPixmapItem* bottom_part;

};

#endif
