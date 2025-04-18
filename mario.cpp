#include "mario.h"
#include <QTimer>
#include <QBrush>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "goomba.h"
#include <QPixmap>
#include "pipe.h"
#include "flag.h"
#include <QMessageBox>


Mario::Mario(int x, int y, QGraphicsScene* scene) {
    jumpSound = new QSoundEffect(this);
    jumpSound->setVolume(1);
    jumpSound->setSource(QUrl("qrc:/graphics/Mario Game Assets/smb_jump-small.wav"));

    goombaHitSound = new QSoundEffect(this);
    goombaHitSound->setVolume(1);
    goombaHitSound->setSource(QUrl("qrc:/graphics/Mario Game Assets/smb_fireball.wav"));

    setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Small_Idle_Right.png"));
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

    QTimer* flagCollisionTimer = new QTimer(this);
    connect(flagCollisionTimer, &QTimer::timeout, this, &Mario::checkFlagCollision);
    flagCollisionTimer->start(16);

    runAnimationTimer = new QTimer(this);
    connect(runAnimationTimer, &QTimer::timeout, this, &Mario::updateAnimation);
    runAnimationTimer->start(100);
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
void Mario::setFinishFlag(Flag* flag) {
    finishFlag = flag;
}

void Mario::updateAnimation() {
    bool leftPressed = pressedKeys.contains(Qt::Key_Left);
    bool rightPressed = pressedKeys.contains(Qt::Key_Right);
    if (!(leftPressed || rightPressed) && onGround) {
        setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Small_Idle_" + horizontalDirection +".png"));
        return;
    } else if (!onGround) {
        setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Small_Jump_" + horizontalDirection +".png"));
        return;
    } else if (leftPressed && rightPressed) {
        setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Small_Idle_" + horizontalDirection +".png"));
        return;
    }

    currentRunFrame = (currentRunFrame + 1) % 3;

    setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Small_Run" + QString::number(currentRunFrame + 1) + "_" + horizontalDirection +".png"));
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
        else if (pressedKeys.contains(Qt::Key_Left) &&
                 marioRect.left()  - eps < pipeTopRect.right() &&
                 marioRect.right() > pipeTopRect.right())
        {
            canMoveLeft = false;
        }
    }
}


void Mario::onFlagSliding(int dy) {
    setY(qMin(y() + dy, 470.0));
}


void Mario::checkFlagCollision() {
    if (winTriggered || !finishFlag)
        return;

    // get the actual pixmap item for the flag cloth

    auto poleSprite = finishFlag->getFlag();

    // see if Mario is really touching that pixmap
    if (collidingItems().contains(poleSprite)) {
        winTriggered = true;

        // 1) stop Mario’s movement
        gravityTimer->stop();
        dynamicObstaclesTimer->stop();
        runAnimationTimer->stop();

        // 2) start the slide‑down
        finishFlag->startFlagAnimation();

        // 3) ride the flag down
        connect(finishFlag, &Flag::sliding, this, &Mario::onFlagSliding);

        // 4) at the end, pop the “You Win!” dialog
        connect(finishFlag, &Flag::animationFinished, this, [](){
            QMessageBox::information(nullptr,
                                     "You Win!",
                                     "Congratulations! You reached the flag!");
        });
    }
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
                goombaHitSound->play();
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
    if (winTriggered) return;   

    pressedKeys.insert(event->key());
    isCollidingWithPipes();

    if (pressedKeys.contains(Qt::Key_Up) && pressedKeys.contains(Qt::Key_Left)) {
        if (onGround) {
            velocityY = -7.1;
            setPos(x() - 7.1, y());
            onGround = false;
            jumpSound->play();
        } else {
            setPos(x() - 10, y());
        }
        horizontalDirection = "Left";
    } else if (pressedKeys.contains(Qt::Key_Up) && pressedKeys.contains(Qt::Key_Right)) {
        if (onGround) {
            velocityY = -7.1;
            setPos(x() + 7.1, y());
            onGround = false;
            jumpSound->play();
        } else {
            setPos(x() + 10, y());
        }
        horizontalDirection = "Right";
    } else if (pressedKeys.contains(Qt::Key_Left) && canMoveLeft) {
        setPos(x() - 10, y());
        horizontalDirection = "Left";
    } else if (pressedKeys.contains(Qt::Key_Right) && canMoveRight) {
        setPos(x() + 10, y());
        horizontalDirection = "Right";
    } else if (pressedKeys.contains(Qt::Key_Space) || pressedKeys.contains(Qt::Key_Up)) {
        if (onGround) {
            velocityY = -10.0;
            onGround = false;
            jumpSound->play();
        }
    }

}

void Mario::keyReleaseEvent(QKeyEvent *event)
{
    pressedKeys.remove(event->key());
}
