#ifndef BOSS_H
#define BOSS_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include "pipe.h"
#include "platform.h"

class Boss :  public QObject, public QGraphicsPixmapItem
{
public:
    int getHeight();
    Boss(int x, int y, QGraphicsScene* sceneInput);
    void setPipes(const QList<pipe*>& pipes);
    void reposition();
private:
    void updateAnimation();
    QTimer* runAnimationTimer;
    double scale = 5.0;
    int height = 32 * scale;
    int currentRunFrame = 1;
    double velocityY = 0;
    double gravity = 0.4;
    bool onGround = false;
    QTimer* positionTimer;
    QList<pipe*> pipeList;
    QGraphicsScene* scene;
    int initX;
    int initY;
    const double acceleration = 0.04;
    const double maxSpeed     = 6.0;
    double VelocityX;

private slots:
    void updatePosition();
};

#endif // BOSS_H
