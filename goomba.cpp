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
}

void Goomba::setPlatforms(const QList<QGraphicsItem*>& platforms) {
    platformList = platforms;
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

void Goomba::move() {
    setPos(x() + 1 * direction, y());
    moved++;
    if (moved >= to_move) {
        direction *= -1;
        moved = 0;
    }
}

int Goomba::getHeight() {
    return height;
}
