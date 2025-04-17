#ifndef GOOMBA_H
#define GOOMBA_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsView>
#include <QList>

class Goomba : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Goomba(int x, int y);
    void setPlatforms(const QList<QGraphicsItem*>& platforms);
    int getHeight();

private:
    QList<QGraphicsItem*> platformList;
    QTimer* gravityTimer;
    QTimer* moveTimer;
    double velocityY = 0;
    double gravity = 0.4;
    double scale = 2.0;
    int to_move = 200;
    int moved = 0;
    int direction = 1;
    int height = 16 * scale;
    bool onGround = false;
private slots:
    void move();
    void applyGravity();
};

#endif // GOOMBA_H
