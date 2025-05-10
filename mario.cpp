#include "mario.h"
#include "platform.h"
#include <QTimer>
#include <QBrush>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "goomba.h"
#include "koopatroopa.h"
#include <QPixmap>
#include "pipe.h"
#include "flag.h"
#include "mushroom.h"
#include "star.h"
#include <QMessageBox>
#include <QGraphicsEffect>


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
    center=new QPointF;
    *center=this->pos();

    jumpSound = new QSoundEffect(this);
    jumpSound->setVolume(1);
    jumpSound->setSource(QUrl("qrc:/graphics/Mario Game Assets/smb_jump-small.wav"));

    goombaHitSound = new QSoundEffect(this);
    goombaHitSound->setVolume(1);
    goombaHitSound->setSource(QUrl("qrc:/graphics/Mario Game Assets/smb_fireball.wav"));

    stagewinSound = new QSoundEffect(this);
    stagewinSound->setVolume(1);
    stagewinSound->setSource(QUrl("qrc:/graphics/Mario Game Assets/Stage Win.wav"));

    mariodeathSound = new QSoundEffect(this);
    mariodeathSound->setVolume(1);
    mariodeathSound->setSource(QUrl("qrc:/graphics/Mario Game Assets/Mario Death.wav"));

    setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Small_Idle_Right.png"));
    setScale(scale);
    setPos(y, x);
    currentScene = scene;

    damageCoolDownTimer = new QTimer(this);
    damageCoolDownTimer->setSingleShot(true);
    connect(damageCoolDownTimer, &QTimer::timeout, this, &Mario::canTakeDamageTruthify);

    allTimer = new QTimer(this);
    connect(allTimer, &QTimer::timeout, this, &Mario::timerFunctions);
    allTimer->start(16);

    runAnimationTimer = new QTimer(this);
    connect(runAnimationTimer, &QTimer::timeout, this, &Mario::updateAnimation);
    runAnimationTimer->start(100);
}

void Mario::timerFunctions() {
    updatePosition();
    isCollidingWithDynamicObstacles();
    getPowerup();
    checkFlagCollision();
    isCollidingWithPipes();
}

int Mario::getHealth() const {
    return health;
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

void Mario::setPipes(const QList<pipe*>& pipes) {
    pipeList = pipes;
}
void Mario::setFinishFlag(Flag* flag) {
    finishFlag = flag;
}

void Mario::getPowerup() {
    QList<QGraphicsItem*> collisions = collidingItems();
    for (QGraphicsItem* collision : collisions) {
        Mushroom* mushroom = dynamic_cast<Mushroom*>(collision);
        Star* star = dynamic_cast<Star*>(collision);
        if (mushroom) {
            becomeSuper();
            delete mushroom;
            return;
        }
        if (star) {
            starman();
            delete star;
            return;
        }
    }
}

void Mario::starman() {
    be_star = true;
    glowEffect = new QGraphicsColorizeEffect(this);
    glowEffect->setColor(Qt::yellow);
    glowEffect->setStrength(0.8);
    this->setGraphicsEffect(glowEffect);
    QTimer::singleShot(5000, this, [this]() {
        be_star = false;
        setGraphicsEffect(nullptr);
    });
}

void Mario::becomeSuper() {
    playerState = "super";
    height = 32 * scale;
    damageToTake = 0;
}

void Mario::becomeBase() {
    be_star = false;
    setGraphicsEffect(nullptr);
    playerState = "base";
    height = 16 * scale;
    damageToTake = 20;
}

void Mario::updateAnimation() {
    bool leftPressed = pressedKeys.contains(Qt::Key_Left);
    bool rightPressed = pressedKeys.contains(Qt::Key_Right);
    if (playerState == "base") {
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
    } else if (playerState == "super") {
        if (!(leftPressed || rightPressed) && onGround) {
            setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Big_Idle_" + horizontalDirection +".png"));
            return;
        } else if (!onGround) {
            setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Big_Jump_" + horizontalDirection +".png"));
            return;
        } else if (leftPressed && rightPressed) {
            setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Big_Idle_" + horizontalDirection +".png"));
            return;
        }
        currentRunFrame = (currentRunFrame + 1) % 3;
        setPixmap(QPixmap(":graphics/Mario Game Assets/Mario_Big_Run" + QString::number(currentRunFrame + 1) + "_" + horizontalDirection +".png"));
    }
}

bool Mario::isStar()  {
    return be_star;
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
    setY(qMin(y() + dy, 500.0 - height));
}

void Mario::checkFlagCollision() {
    if (winTriggered || !finishFlag)
        return;
    auto poleSprite = finishFlag->getFlag();
    if (collidingItems().contains(poleSprite)) {
        winTriggered = true;
        allTimer->stop();
        runAnimationTimer->stop();
        stagewinSound->play();
        finishFlag->startFlagAnimation();
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
    if(this->pos().x()>center->x()){
        *center=this->pos();
    }
    if (y() > 700 && !winTriggered) {
        takeDamage(1000);
        goombaHitSound->play();
        return;
    }
    QList<QGraphicsItem*> collisions = collidingItems();
    for (QGraphicsItem* collision : collisions) {
        if (dynamic_cast<Platform*>(collision)) {
            if (velocityY > 0) {
                setPos(x(), collision->y() - height);
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
        if(this->x()>center->x()-400) velocityX -= acceleration;
        else velocityX = 0;
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
        KoopaTroopa* koopa = dynamic_cast<KoopaTroopa*>(collision);
        if (goomba) {
            if (isStar()) {
                currentScene->removeItem(goomba);
                delete goomba;
                score += 100;
                goombaHitSound->play();
            } else {
                double marioBottom = y() + height;
                double goombaTop = goomba->y();
                if (marioBottom <= goombaTop + goomba->getHeight() / 2 && velocityY >= 0) {
                    jumpForce = 7;
                    jump();
                    jumpSound->play();
                    onGround = false;
                    currentScene->removeItem(goomba);
                    delete goomba;
                    score += 100;
                    goombaHitSound->play();
                } else if (canTakeDamage) {
                    takeDamage(damageToTake);
                    becomeBase();
                    goombaHitSound->play();
                }
            }
        } else if (koopa && koopa->getStatus() == "monitor") {
            if (isStar()) {
                koopa->becomeCrazy();
                score += 100;
                goombaHitSound->play();
                takeDamage(0);
            } else {
                double marioBottom = y() + height;
                double koopaTop = koopa->y();
                if (marioBottom <= koopaTop + koopa->getHeight() / 2 && velocityY >= 0) {
                    jumpForce = 7;
                    jump();
                    jumpSound->play();
                    onGround = false;
                    koopa->becomeCrazy();
                    score += 100;
                    goombaHitSound->play();
                    takeDamage(0); // DO NOT REMOVE! THIS GIVES THE PLAYER SOME TIME TO ESCAPE!
                } else if (canTakeDamage) {
                    takeDamage(damageToTake);
                    becomeBase();
                    goombaHitSound->play();
                }
            }
        } else if (koopa && koopa->getStatus() == "crazy") {
            if (isStar()) {
                goombaHitSound->play();
                takeDamage(0);
            } else {
                double marioBottom = y() + height;
                double koopaTop = koopa->y();
                if (marioBottom <= koopaTop + koopa->getHeight() / 2 && velocityY >= 0) {
                    jumpForce = 7;
                    jump();
                    jumpSound->play();
                    onGround = false;
                    goombaHitSound->play();
                    takeDamage(0); // DO NOT REMOVE! THIS GIVES THE PLAYER SOME TIME TO ESCAPE!
                } else if (canTakeDamage) {
                    takeDamage(damageToTake);
                    becomeBase();
                    goombaHitSound->play();
                }
            }
        }
    }
}

void Mario::takeDamage(int amount) {
    if(isStar() && amount != 1000) return;
    if (!canTakeDamage && amount != 1000) return;
    canTakeDamage = false;
    health -= amount;
    damageCoolDownTimer->start(1000);
    if (health <= 0) {
        becomeBase();
        lives--;
        health = 100;
        if (lives <= 0) {
            QMessageBox::information(nullptr, "Game Over", "You lost all your lives!");
            currentScene->clear();
            return;
        }
        setPos(120,450);
        *center=this->pos();
    }
}

void Mario::keyPressEvent(QKeyEvent *event)
{
    if (winTriggered) return;   

    pressedKeys.insert(event->key());
    isCollidingWithPipes();
    switch(event->key()) {
    case Qt::Key_Left:
        isMovingLeft = true;
        horizontalDirection = "Left";
        break;
    case Qt::Key_Right:
        isMovingRight = true;
        horizontalDirection = "Right";
        break;
    case Qt::Key_Space:
    case Qt::Key_Up:
        if (onGround && !isJumping) {
            jumpForce = 15;
            jump();
            jumpSound->play();
        }
        break;
    default:
        QGraphicsPixmapItem::keyPressEvent(event);
        break;
    }
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
