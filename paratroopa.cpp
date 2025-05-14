#include "paratroopa.h"
#include <QGraphicsScene>

Paratroopa::Paratroopa(int x, int y) {
    setPixmap(QPixmap(":/graphics/Mario Game Assets/Paratroopa_1.png"));
    setScale(scale);
    setPos(y, x);

    positionTimer = new QTimer(this);
    connect(positionTimer, &QTimer::timeout, this, &Paratroopa::updatePosition);
    positionTimer->start(16);

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &Paratroopa::updateAnimation);
    animationTimer->start(100);
}

void Paratroopa::updateAnimation() {
    frame = (frame + 1) % 2;
    if (direction == -1)
        setPixmap(QPixmap(":/graphics/Mario Game Assets/Paratroopa_" + QString::number(frame + 1) +"_Right.png"));
    else {
        setPixmap(QPixmap(":/graphics/Mario Game Assets/Paratroopa_" + QString::number(frame + 1) +"_Left.png"));
    }
}

void Paratroopa::updatePosition() {
    jumpTimer++;
    if (jumpTimer >= jumpInterval) {
        velocityY = jumpStrength;
        jumpTimer = 0;
    }
    velocityY += gravity;

    QList<QGraphicsItem*> collisions = collidingItems();
    onGround = false;

    for (QGraphicsItem* platform : platformList) {
        if (collisions.contains(platform)) {
            double bottom = y() + height;
            double top = platform->y() + platform->boundingRect().height() / 2;
            if (bottom <= top && velocityY >= 0) {
                setPos(x(), platform->y() - height);
                velocityY = 0;
                onGround = true;
                break;
            }
        }
    }

    isCollidingWithPipes();

    velocityX = 2;
    if (!canMove || (moved >= to_move)) {
        direction *= -1;
        moved = 0;
    }
    setPos(x() + velocityX * direction, y() + velocityY);
    moved++;
}




void Paratroopa::isCollidingWithPipes() {
    canMove  = true;
    const qreal eps = 2.0;

    QRectF Paratroopa = this->sceneBoundingRect();

    for (pipe* p : pipeList) {
        QGraphicsPixmapItem* topPart = p->getTopPart();
        QRectF pipeTopRect = topPart->sceneBoundingRect();

        if ( Paratroopa.right() + eps > pipeTopRect.left() && Paratroopa.left() < pipeTopRect.left())
        {
            canMove = false;
        }
        else if ( Paratroopa.left()  - eps < pipeTopRect.right() && Paratroopa.right() > pipeTopRect.right())
        {
            canMove = false;
        }
    }
}

void Paratroopa::turnIntoKoopa() {
    KoopaTroopa* koopa = new KoopaTroopa(y(), x());
    koopa->setPlatforms(platformList);
    koopa->setPipes(pipeList);
    scene()->addItem(koopa);
    scene()->removeItem(this);
    delete this;
}

void Paratroopa::setPlatforms(const QList<Platform*>& platforms) {
    platformList = platforms;
}

void Paratroopa::setPipes(const QList<pipe*>& pipes) {
    pipeList = pipes;
}

int Paratroopa::getHeight() const {
    return height;
}
