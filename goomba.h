#ifndef GOOMBA_H
#define GOOMBA_H

#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsView>
#include <QList>

class Goomba : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Goomba(int x, int y);
    void setPlatforms(const QList<QGraphicsItem*>& platforms);

private:
    QList<QGraphicsItem*> platformList;
    QTimer* gravityTimer;
    QTimer* moveTimer;
    double velocityY = 0;
    double gravity = 0.4;

    int to_move = 200;
    int moved = 0;
    int direction = 1;

    bool onGround = false;
private slots:
    void move();
    void applyGravity();
};

#endif // GOOMBA_H
