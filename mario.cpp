#include "mario.h"
#include <QTimer>
#include <QBrush>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "goomba.h"
#include <QPixmap>

Mario::Mario(int x, int y, QGraphicsScene* scene) {

    setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Small_Idle.png"));
    setScale(scale);
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
                setPos(x(), platform->y() - height);
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
        Goomba* goomba = dynamic_cast<Goomba*>(collision);
        if (goomba) {
            double marioBottom = y() + height;
            double goombaTop = goomba->y();

            if (marioBottom <= goombaTop + 5) {
                velocityY = -10.0;
                onGround = false;
                currentScene->removeItem(goomba);
                delete goomba;
            } else if (canTakeDamage) {
                lives--;
                canTakeDamage = false;
                damageCoolDownTimer->start(1000);
                velocityY = -15.0;
                onGround = false;

                if (lives == 0) {
                    currentScene->clear();
                }
            }
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
