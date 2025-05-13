#include "boss.h"
#include "platform.h"
#include <QTimer>
#include "pipe.h"
#include "mario.h"

Boss::Boss(int x, int y, QGraphicsScene* sceneInput) {
    setPixmap(QPixmap(":graphics/Mario Game Assets/Bowser1.png"));
    setScale(scale);
    setPos(x, y);
    initX = x;
    initY = y;
    scene = sceneInput;
    positionTimer = new QTimer(this);
    connect(positionTimer, &QTimer::timeout, this, &Boss::updatePosition);
    positionTimer->start(16);
    runAnimationTimer = new QTimer(this);
    connect(runAnimationTimer, &QTimer::timeout, this, &Boss::updateAnimation);
    runAnimationTimer->start(100);
}

void Boss::updateAnimation() {
    currentRunFrame = (currentRunFrame + 1) % 2;
    setPixmap(QPixmap(":graphics/Mario Game Assets/Bowser" + QString::number(currentRunFrame + 1) +".png"));
}

void Boss::updatePosition() {
    velocityY += gravity;

    QList<QGraphicsItem*> collisions = collidingItems();
    onGround = false;

    for (QGraphicsItem* collision : collisions) {
        if (dynamic_cast<Platform*>(collision)) {
            setPos(x(), collision->y() - height);
            onGround = true;
            velocityY = 0;
            break;
        }
    }

    setPos(x() + 4, y() + velocityY);
}


int Boss::getHeight() {
    return height;
}

void Boss::setPipes(const QList<pipe*>& pipes) {
    pipeList = pipes;
}

void Boss::reposition() {
    setPos(initX, initY);
}
