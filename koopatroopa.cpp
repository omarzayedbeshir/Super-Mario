#include "koopatroopa.h"
#include <QTimer>
#include <QBrush>
#include <QGraphicsView>

KoopaTroopa::KoopaTroopa(int x, int y) {
    setPixmap(QPixmap(":graphics/Mario Game Assets/Koopa_Walk1_Right.png"));
    setScale(scale);
    setPos(y, x);

    gravityTimer = new QTimer(this);
    connect(gravityTimer, &QTimer::timeout, this, &KoopaTroopa::applyGravity);

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &KoopaTroopa::move);

    gravityTimer->start(16);
    moveTimer->start(16);

    runAnimationTimer = new QTimer(this);
    connect(runAnimationTimer, &QTimer::timeout, this, &KoopaTroopa::updateAnimation);
    runAnimationTimer->start(100);
}

void KoopaTroopa::updateAnimation() {
    if (status == "monitor") {
        currentRunFrame = (currentRunFrame + 1) % 2;

        if (direction == 1)
            setPixmap(QPixmap(":graphics/Mario Game Assets/Koopa_Walk" + QString::number(currentRunFrame + 1) +"_Right.png"));
        else {
            setPixmap(QPixmap(":graphics/Mario Game Assets/Koopa_Walk" + QString::number(currentRunFrame + 1) +"_Left.png"));
        }
    }
}

void KoopaTroopa::setPlatforms(const QList<QGraphicsItem*>& platforms) {
    platformList = platforms;
}


void KoopaTroopa::setPipes(const QList<pipe*>& pipes) {
    pipeList = pipes;
}

void KoopaTroopa::applyGravity() {
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

void KoopaTroopa::isCollidingWithPipes()
{
    canMove  = true;
    const qreal eps = 2.0;

    QRectF koopaTroopaRect = this->sceneBoundingRect();

    for (pipe* p : pipeList) {
        QGraphicsPixmapItem* topPart = p->getTopPart();
        QRectF pipeTopRect = topPart->sceneBoundingRect();

        if ( koopaTroopaRect.right() + eps > pipeTopRect.left() && koopaTroopaRect.left() < pipeTopRect.left())
        {
            canMove = false;
        }
        else if ( koopaTroopaRect.left()  - eps < pipeTopRect.right() && koopaTroopaRect.right() > pipeTopRect.right())
        {
            canMove = false;
        }
    }
}
void KoopaTroopa::move() {

    isCollidingWithPipes();
    if (status == "monitor") {
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
    } else {
        setPos(x() + 10, y());
    }
}

int KoopaTroopa::getHeight() {
    return height;
}

void KoopaTroopa::becomeCrazy() {
    setPixmap(QPixmap(":graphics/Mario Game Assets/Koopa_Shell.png"));
    status = "crazy";
}

QString KoopaTroopa::getStatus() {
    return status;
}
