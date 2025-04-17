#include "mario.h"
#include <QTimer>
#include <QBrush>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "goomba.h"

Mario::Mario(int x, int y, QGraphicsScene* scene) {
    setRect(0, 0, 50, 50);
    setPos(y, x);
    currentScene = scene;
    gravityTimer = new QTimer(this);
    connect(gravityTimer, &QTimer::timeout, this, &Mario::applyGravity);
    gravityTimer->start(16);

    dynamicObstaclesTimer = new QTimer(this);
    connect(dynamicObstaclesTimer, &QTimer::timeout, this, &Mario::isCollidingWithDynamicObstacles);
    dynamicObstaclesTimer->start(16);

    damageCoolDownTimer = new QTimer(this);
    damageCoolDownTimer->setSingleShot(true);
    connect(damageCoolDownTimer, &QTimer::timeout, this, &Mario::canTakeDamageTruthify);
    setBrush(Qt::white);
}

void Mario::canTakeDamageTruthify() {
    canTakeDamage = true;
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

void Mario::isCollidingWithDynamicObstacles() {
    QList<QGraphicsItem*> collisions = collidingItems();

    for (QGraphicsItem* collision : collisions) {
        if (dynamic_cast<Goomba*>(collision) && canTakeDamage) {
            velocityY = -10.0;
            lives--;
            onGround = false;
            canTakeDamage = false;
            damageCoolDownTimer->start(1000);

            if (lives == 0) {
                currentScene->clear();
            }
            break;
        }
    }
}

void Mario::keyPressEvent(QKeyEvent *event)
{
    pressedKeys.insert(event->key());

    if (pressedKeys.contains(Qt::Key_Up) && pressedKeys.contains(Qt::Key_Left)) {
        if (onGround) {
            velocityY = -7.1;
            setPos(x() - 7.1, y());
            onGround = false;
        } else {
            setPos(x() - 10, y());
        }
    } else if (pressedKeys.contains(Qt::Key_Up) && pressedKeys.contains(Qt::Key_Right)) {
        if (onGround) {
            velocityY = -7.1;
            setPos(x() + 7.1, y());
            onGround = false;
        } else {
            setPos(x() + 10, y());
        }

    } else if (pressedKeys.contains(Qt::Key_Left)) {
        setPos(x() - 10, y());
    } else if (pressedKeys.contains(Qt::Key_Right)) {
        setPos(x() + 10, y());
    } else if (pressedKeys.contains(Qt::Key_Space) || pressedKeys.contains(Qt::Key_Up)) {
        if (onGround) {
            velocityY = -10.0;
            onGround = false;
        }
    }
}

void Mario::keyReleaseEvent(QKeyEvent *event)
{
    pressedKeys.remove(event->key());
}
