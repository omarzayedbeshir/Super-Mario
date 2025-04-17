#include "mario.h"
#include <QTimer>
#include <QBrush>
#include <QGraphicsView>

Mario::Mario(int x, int y) {
    setRect(0, 0, 50, 50);
    setPos(y, x);
    gravityTimer = new QTimer(this);
    connect(gravityTimer, &QTimer::timeout, this, &Mario::applyGravity);
    gravityTimer->start(16);
    setBrush(Qt::white);
}

void Mario::setPlatforms(const QList<QGraphicsItem*>& platforms) {
    platformList = platforms;
}

void Mario::applyGravity() {
    velocityY += gravity;

    double dy = velocityY;
    setPos(x(), y() + dy);

    QList<QGraphicsItem*> collisions = collidingItems();
    onGround = false;

    for (QGraphicsItem* platform : platformList) {
        if (collisions.contains(platform)) {
            if (velocityY > 0) {
                setPos(x(), platform->y() - rect().height());
                onGround = true;
                velocityY = 0;
            }
            break;
        }
    }
}

void Mario::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        setPos(x() - 10, y());
        break;
    case Qt::Key_Right:
        setPos(x() + 10, y());
        break;
    case Qt::Key_Up:
    case Qt::Key_Space:
        if (onGround) {
            velocityY = -10.0;
            onGround = false;
        }
        break;
    }
}
