#include "mario.h"
#include <QTimer>
#include <QBrush>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "goomba.h"
#include <QPixmap>
#include "pipe.h"


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

    QTimer* pipeCollisionTimer = new QTimer(this);
    connect(pipeCollisionTimer, &QTimer::timeout, this, &Mario::isCollidingWithPipes);
    pipeCollisionTimer->start(16);
}

void Mario::canTakeDamageTruthify() {
    canTakeDamage = true;
}

int Mario::getLives() {
    return lives;
}

int Mario::getScore() {
    return score;
}

void Mario::setPlatforms(const QList<QGraphicsItem*>& platforms) {
    platformList = platforms;
}

void Mario::setPipes(const QList<pipe*>& pipes) {
    pipeList = pipes;
}



void Mario::isCollidingWithPipes()
{
    const qreal collisionThreshold = 5.0;
    canMoveRight = true;
    canMoveLeft  = true;

    const qreal eps = 4.0;


    for (pipe* p : pipeList) {
        QGraphicsPixmapItem* topPart = p->getTopPart();
        QRectF pipeTopRect = topPart->sceneBoundingRect();
        QRectF marioRect = this->sceneBoundingRect();

        bool verticalCollision =
            (marioRect.bottom() >= pipeTopRect.top() - collisionThreshold) &&
            (marioRect.bottom() <= pipeTopRect.top() + collisionThreshold);

        bool horizontalOverlap =
            marioRect.right() > pipeTopRect.left() &&
            marioRect.left() < pipeTopRect.right();

        if (verticalCollision && horizontalOverlap && velocityY >= 0) {
            setPos(x(), pipeTopRect.top() - marioRect.height());
            onGround = true;
            velocityY = 0;
            return;
        }

        if (pressedKeys.contains(Qt::Key_Right) &&
            marioRect.right() + eps > pipeTopRect.left() &&
            marioRect.left() < pipeTopRect.left())
        {
            canMoveRight = false;
        }
        // if Mario's left side is moving into the pipe's right side
        else if (pressedKeys.contains(Qt::Key_Left) &&
                 marioRect.left()  - eps < pipeTopRect.right() &&
                 marioRect.right() > pipeTopRect.right())
        {
            canMoveLeft = false;
        }



    }
}



void Mario::applyGravity() {
    velocityY += gravity;

    double dy = velocityY;
    setPos(x(), y() + dy);

    QList<QGraphicsItem*> collisions = collidingItems();
    onGround = false;

    isCollidingWithPipes(); // New pipe collision check

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





void Mario::isCollidingWithDynamicObstacles()
{
    QList<QGraphicsItem*> collisions = collidingItems();

    for (QGraphicsItem* collision : collisions) {
        Goomba* goomba = dynamic_cast<Goomba*>(collision);
        if (goomba) {
            double marioBottom = y() + height;
            double goombaTop = goomba->y();

            if (marioBottom <= goombaTop + goomba->getHeight() / 2) {
                //velocityY = -1.0;
                onGround = false;
                currentScene->removeItem(goomba);
                delete goomba;
                score += 100;
            } else if (canTakeDamage) {
                lives--;
                canTakeDamage = false;
                damageCoolDownTimer->start(1500);
                //velocityY = -15.0;
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
    } else if (pressedKeys.contains(Qt::Key_Left) && canMoveLeft) {
            setPos(x() - 10, y());
    } else if (pressedKeys.contains(Qt::Key_Right) && canMoveRight) {
            setPos(x() + 10, y());    } else if (pressedKeys.contains(Qt::Key_Space) || pressedKeys.contains(Qt::Key_Up)) {
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
