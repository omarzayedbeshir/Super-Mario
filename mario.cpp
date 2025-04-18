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


Mario::Mario(int x, int y, QGraphicsScene* scene):
    velocityX(0),
    velocityY(0),
    gravity(0.8),
    jumpForce(15),
    acceleration(0.3),
    maxSpeed(5),
    friction(0.3),
    isMovingLeft(false),
    isMovingRight(false),
    isJumping(false),
    isFacingRight(true),
    onGround(false)
{
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
    connect(gravityTimer, &QTimer::timeout, this, &Mario::updatePosition);
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
    canMoveRight = true;
    canMoveLeft = true;

    QRectF marioRect = this->sceneBoundingRect();

    for (pipe* p : pipeList) {
        QGraphicsPixmapItem* topPart = p->getTopPart();
        QRectF pipeTopRect = topPart->sceneBoundingRect();

        QGraphicsPixmapItem* bodyPart = p->getBottomPart();
        QRectF pipeBodyRect = bodyPart->sceneBoundingRect();

        bool landingOnTop =
            (marioRect.bottom() >= pipeTopRect.top() - 5) &&
            (marioRect.bottom() <= pipeTopRect.top() + 5) &&
            (marioRect.right() > pipeTopRect.left() + 5) &&
            (marioRect.left() < pipeTopRect.right() - 5) &&
            velocityY >= 0;

        if (landingOnTop) {
            setPos(x(), pipeTopRect.top() - marioRect.height());
            onGround = true;
            velocityY = 0;
            continue;
        }

        if (marioRect.intersects(pipeTopRect) && marioRect.bottom() > pipeTopRect.top() + 5) {
            handleSideCollision(marioRect, pipeTopRect);
        }

        if (marioRect.intersects(pipeBodyRect)) {
            handleSideCollision(marioRect, pipeBodyRect);
        }

        preventApproaching(marioRect, pipeTopRect);

        preventApproaching(marioRect, pipeBodyRect);
    }
}

void Mario::handleSideCollision(const QRectF& marioRect, const QRectF& obstacleRect)
{
    double leftOverlap = marioRect.right() - obstacleRect.left();
    double rightOverlap = obstacleRect.right() - marioRect.left();

    if (leftOverlap < rightOverlap && leftOverlap > 0) {
        setPos(obstacleRect.left() - marioRect.width(), y());
        canMoveRight = false;
    } else if (rightOverlap > 0) {
        setPos(obstacleRect.right(), y());
        canMoveLeft = false;
    }
}

void Mario::preventApproaching(const QRectF& marioRect, const QRectF& obstacleRect)
{
    if (!marioRect.intersects(obstacleRect)) {
        QRectF nextPosRight = marioRect;
        nextPosRight.translate(10, 0);
        if (nextPosRight.intersects(obstacleRect)) {
            canMoveRight = false;
        }

        QRectF nextPosLeft = marioRect;
        nextPosLeft.translate(-10, 0);
        if (nextPosLeft.intersects(obstacleRect)) {
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


    auto poleSprite = finishFlag->getFlag();

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

        connect(finishFlag, &Flag::animationFinished, this, [](){
            QMessageBox::information(nullptr,
                                     "You Win!",
                                     "Congratulations! You reached the flag!");
        });
    }
}

void Mario::jump()
{
    velocityY = -jumpForce;
    isJumping = true;
    onGround = false;
}

void Mario::updatePosition() {
    applyPhysics();
    setPos(x() + velocityX, y() + velocityY);

    if (y() > 700 && !winTriggered) {
        resetAfterDeath();
        setPos(120, 450);
        return;
    }

    QList<QGraphicsItem*> collisions = collidingItems();
    for (QGraphicsItem* platform : platformList) {
        if (collisions.contains(platform)) {
            if (velocityY > 0) {
                setPos(x(), platform->y() - height);
                onGround = true;
                isJumping=false;
            }
            return;
        }
    }
    onGround=false;
}

void Mario::applyPhysics()
{

    if(isMovingRight==isMovingLeft){
        if (velocityX > 0) {
            velocityX -= friction;
            if (velocityX < 0) velocityX = 0;
        } else if (velocityX < 0) {
            velocityX += friction;
            if (velocityX > 0) velocityX = 0;
        }
    }
    else if (isMovingRight) {
        velocityX += acceleration;
        isFacingRight = true;
    }
    else if (isMovingLeft) {
        velocityX -= acceleration;
        isFacingRight = false;
    }

    if (velocityX > maxSpeed) {
        velocityX = maxSpeed;
    } else if (velocityX < -maxSpeed) {
        velocityX = -maxSpeed;
    }

    if (!onGround) {
        velocityY += gravity;
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
                canTakeDamage = false;
                damageCoolDownTimer->start(1500);
                resetAfterDeath();
            }
        }
    }
}

void Mario::resetAfterDeath() {
    lives--;

    if (lives <= 0) {
        QMessageBox::information(nullptr, "Game Over", "You lost all your lives!");
        currentScene->clear();
        return;
    }

    // Reset position to start (adjust to your starting point)
    // Reset state
    velocityY = 0;
    onGround = false;
    pressedKeys.clear();
}


void Mario::keyPressEvent(QKeyEvent *event)
{
    if (winTriggered) return;   

    pressedKeys.insert(event->key());
    isCollidingWithPipes();
    switch(event->key()) {
    case Qt::Key_Left:
        //if(!isJumping) isMovingLeft = true;
        isMovingLeft = true;
        horizontalDirection = "Left";
        break;
    case Qt::Key_Right:
        //if(!isJumping) isMovingRight = true;
        isMovingRight = true;
        horizontalDirection = "Right";
        break;
    case Qt::Key_Space:
    case Qt::Key_Up:
        if (onGround && !isJumping) {
            jump();
            jumpSound->play();
        }
        break;
    default:
        QGraphicsPixmapItem::keyPressEvent(event);
        break;
    }
    /*if (pressedKeys.contains(Qt::Key_Up) && pressedKeys.contains(Qt::Key_Left)) {
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
    }*/

}

void Mario::keyReleaseEvent(QKeyEvent *event)
{
    pressedKeys.remove(event->key());
    switch(event->key()) {
    case Qt::Key_Left:
        isMovingLeft = false;
        break;
    case Qt::Key_Right:
        isMovingRight = false;
        break;
    case Qt::Key_Space:
    case Qt::Key_Up:
        isJumping = false;
        break;
    default:
        QGraphicsPixmapItem::keyReleaseEvent(event);
        break;
    }
}
