#include "koopatroopa.h"
#include "platform.h"
#include <QTimer>
#include <QBrush>
#include <QGraphicsView>

KoopaTroopa::KoopaTroopa(int x, int y) {
    setPixmap(QPixmap(":graphics/Mario Game Assets/Koopa_Walk1_Right.png"));
    setScale(scale);
    setPos(y, x);

    positionTimer = new QTimer(this);
    connect(positionTimer, &QTimer::timeout, this, &KoopaTroopa::updatePosition);
    positionTimer->start(16);

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

void KoopaTroopa::setPlatforms(const QList<Platform*>& platforms) {
    platformList = platforms;
}

void KoopaTroopa::setPipes(const QList<pipe*>& pipes) {
    pipeList = pipes;
}

void KoopaTroopa::updatePosition() {
    velocityY += gravity;

    QList<QGraphicsItem*> collisions = collidingItems();
    onGround = false;

    for (QGraphicsItem* platform : platformList) {
        if (collisions.contains(platform)) {
            setPos(x(), platform->y() - height);
            onGround = true;
            velocityY = 0;
            break;
        }
    }

    isCollidingWithPipes();

    if (status == "monitor") velocityX = 1;
    else velocityX = 5;
    if (!canMove || moved >= to_move) {
        direction *= -1;
        moved = 0;
    }
    setPos(x() + velocityX * direction, y() + velocityY);
    moved++;
}

void KoopaTroopa::isCollidingWithPipes() {
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
