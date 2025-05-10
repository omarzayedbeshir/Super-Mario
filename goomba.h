#ifndef GOOMBA_H
#define GOOMBA_H

#include "pipe.h"
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsView>
#include <QList>
#include "platform.h"

class Goomba :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Goomba(int x, int y);
    void setPlatforms(const QList<Platform*>& platforms);
    void setPipes(const QList<pipe*>& pipes);
    int getHeight();
    void isCollidingWithPipes();


private:
    QList<Platform*> platformList;
    QList<pipe*> pipeList;
    void updateAnimation();
    QTimer* positionTimer;
    QTimer* runAnimationTimer;
    double velocityY = 0;
    double gravity = 0.4;
    double scale = 2.0;
    int to_move = 200;
    int moved = 0;
    int direction = 1;
    int height = 16 * scale;
    bool onGround = false;
    bool canMove;
    int currentRunFrame = 1;
private slots:
    void updatePosition();
};

#endif // GOOMBA_H
