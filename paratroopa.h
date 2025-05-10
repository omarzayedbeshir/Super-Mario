#ifndef PARATROOPA_H
#define PARATROOPA_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include "platform.h"
#include "pipe.h"
#include "koopatroopa.h"

class Paratroopa : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Paratroopa(int x, int y);
    void updateAnimation();
    void updatePosition();
    void turnIntoKoopa();
    void setPlatforms(const QList<Platform*>& platforms);
    void setPipes(const QList<pipe*>& pipes);
    int getHeight() const;
    void isCollidingWithPipes();


private:
    QTimer* positionTimer;
    QTimer* animationTimer;
    QList<Platform*> platformList;
    QList<pipe*> pipeList;
    double velocityY = 0;
    double velocityX = 0;
    double gravity = 0.8;
    double scale = 1.8;
    int jumpStrength = -12;
    int jumpInterval = 48;
    int jumpTimer = 0;
    int to_move = 250;
    int moved = 0;
    int direction = 1;
    int frame = 0;
    bool onGround = false;
    int height = 24 * scale;
    bool canMove;
};
#endif // PARATROOPA_H
