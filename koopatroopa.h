#ifndef KOOPATROOPA_H
#define KOOPATROOPA_H

#include "pipe.h"
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsView>
#include <QList>
#include <QString>
#include "platform.h"

class KoopaTroopa :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    KoopaTroopa(int x, int y);
    void setPlatforms(const QList<Platform*>& platforms);
    void setPipes(const QList<pipe*>& pipes);
    int getHeight();
    void isCollidingWithPipes();
    void becomeCrazy();
    QString getStatus();

private:
    QList<Platform*> platformList;
    QList<pipe*> pipeList;
    void updateAnimation();
    QTimer* positionTimer;
    QTimer* runAnimationTimer;
    double velocityY = 0;
    double velocityX = 1;
    double gravity = 0.8;
    double scale = 2.0;
    int to_move = 200;
    int moved = 0;
    int direction = 1;
    int height = 24 * scale;
    QString status = "monitor"; // monitor, crazy
    bool onGround = false;
    bool canMove;
    int currentRunFrame = 1;
private slots:
    void updatePosition();
};

#endif // KOOPATROOPA_H
