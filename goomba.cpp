#include "goomba.h"
#include <QTimer>
#include <QBrush>
#include <QGraphicsView>

Goomba::Goomba(int x, int y) {
    setPixmap(QPixmap(":graphics/Mario Game Assets/Goomba_Walk1.png"));
    setScale(scale);
    setPos(y, x);

    gravityTimer = new QTimer(this);
    connect(gravityTimer, &QTimer::timeout, this, &Goomba::applyGravity);

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Goomba::move);

    gravityTimer->start(16);
    moveTimer->start(16);

    runAnimationTimer = new QTimer(this);
    connect(runAnimationTimer, &QTimer::timeout, this, &Goomba::updateAnimation);
    runAnimationTimer->start(100);
}

void Goomba::updateAnimation() {
    currentRunFrame = (currentRunFrame + 1) % 2;

    setPixmap(QPixmap(":graphics/Mario Game Assets/Goomba_Walk" + QString::number(currentRunFrame + 1) +".png"));

}

void Goomba::setPlatforms(const QList<QGraphicsItem*>& platforms) {
    platformList = platforms;
}


void Goomba::setPipes(const QList<pipe*>& pipes) {
    pipeList = pipes;
}

void Goomba::applyGravity() {
    velocityY += gravity;

    double dy = velocityY;
    setPos(x(), y() + dy);

    QList<QGraphicsItem*> collisions = collidingItems();
    onGround = false;

    for (QGraphicsItem* platform : platformList) {
        if (collisions.contains(platform)) {
            if (velocityY > 0) {
                setPos(x(), platform->y() - height);
                onGround = true;
                velocityY = 0;
            }
            break;
        }
    }
}

void Goomba::isCollidingWithPipes()
{
    canMove  = true;
    const qreal eps = 2.0;

    QRectF goombaRect = this->sceneBoundingRect();

    for (pipe* p : pipeList) {
        QGraphicsPixmapItem* topPart = p->getTopPart();
        QRectF pipeTopRect = topPart->sceneBoundingRect();

        if ( goombaRect.right() + eps > pipeTopRect.left() && goombaRect.left() < pipeTopRect.left())
        {
            canMove = false;
        }
        else if ( goombaRect.left()  - eps < pipeTopRect.right() && goombaRect.right() > pipeTopRect.right())
        {
            canMove = false;
        }
    }
}
void Goomba::move() {

    isCollidingWithPipes();

    if (!canMove) {
        direction *= -1;
        moved = 0;
        setPos(x() + 1 * direction, y());
    } else {
        setPos(x() + 1 * direction, y());
        moved++;
        if (moved >= to_move) {
            direction *= -1;
            moved = 0;
        }
    }


}

int Goomba::getHeight() {
    return height;
}
