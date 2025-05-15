#ifndef MARIO_H
#define MARIO_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QSet>
#include "platform.h"
#include <QSoundEffect>
#include <QGraphicsColorizeEffect>


class pipe;
class Flag;
class Goomba;

class Mario : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Mario();
    int getLevel() const;
    int getLives() const;
    int getScore() const;
    int getHealth() const;
    void setScoreAtLevelStart(int scoreInput);
    void setLives(int Lives);
    void setScore(int Score);
    void setHealth(int Health);
    void becomeBase();
    void becomeSuper();
    void setScene(QGraphicsScene *scene);
    void setInitPos(int x, int y);
    void setInitMovement();
    void setPipes(const QList<pipe*>& pipes);
    void setFinishFlag(Flag* flag);
    void setGoombas(const QList<Goomba*>& goombas);
    void getPowerup();
    void starman();

    void timerFunctions();



private:
    void handleSideCollision(const QRectF& marioRect, const QRectF& obstacleRect);
    void preventApproaching(const QRectF& marioRect, const QRectF& obstacleRect);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void isCollidingWithDynamicObstacles();
    void canTakeDamageTruthify();
    void isCollidingWithPipes();
    void updateAnimation();
    void takeDamage(int amount);
    bool isStar();
    QTimer* allTimer;
    int initX;
    int initY;

    QSoundEffect* jumpSound;
    QSoundEffect* goombaHitSound;
    QSoundEffect* stagewinSound;
    QSoundEffect* mariodeathSound;

    QGraphicsScene* currentScene;
    QList<pipe*> pipeList;
    QTimer* positionTimer;
    QTimer* dynamicObstaclesTimer;
    QTimer* damageCoolDownTimer;
    QTimer* runAnimationTimer;
    QTimer* glowTimer;

    QPointF *center;

    int currentRunFrame = 0;
    double velocityX;
    double velocityY = 0;
    double gravity = 0.4;
    double jumpForce;
    double acceleration;
    double maxSpeed;
    double friction;
    double scale = 2.0;
    int damageToTake = 20;
    int score = 0;
    int height = 16 * scale;
    bool canTakeDamage = true;
    bool be_star = false;
    int lives = 5;
    int scoreAtLevelStart;
    int health = 100;
    int level;
    bool canMoveRight;
    bool canMoveLeft;
    QString playerState = "base"; // base = 0, super = 1, fire = 2
    Flag* finishFlag = nullptr;
    bool  winTriggered = false;
    bool isMovingLeft;
    bool isMovingRight;
    bool isJumping;
    bool isFacingRight;
    bool onGround;
    bool glowState = false;

    QGraphicsColorizeEffect* glowEffect = nullptr;


    QGraphicsTextItem* healthText;




    QSet<int> pressedKeys;
    QString horizontalDirection = "Right";

private slots:
    void updatePosition();
    void applyPhysics();
    void jump();
    void checkFlagCollision();
    void onFlagSliding(int dy);


};

#endif // MARIO_H
