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
#include <QSoundEffect>


class pipe;
class Flag;
class Goomba;

class Mario : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Mario(int x, int y, QGraphicsScene* scene);
    void setPlatforms(const QList<QGraphicsItem*>& platforms);
    int getLives();
    int getScore();
    int getHealth() const;

    void setPipes(const QList<pipe*>& pipes);
    void setFinishFlag(Flag* flag);
    void setGoombas(const QList<Goomba*>& goombas);
    void getMushroom();


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


    QSoundEffect* jumpSound;
    QSoundEffect* goombaHitSound;
    QSoundEffect* stagewinSound;
    QSoundEffect* mariodeathSound;

    QGraphicsScene* currentScene;
    QList<QGraphicsItem*> platformList;
    QList<QGraphicsItem*> dynamicObstaclesList;
    QList<pipe*> pipeList;
    QTimer* gravityTimer;
    QTimer* dynamicObstaclesTimer;
    QTimer* damageCoolDownTimer;
    QTimer* runAnimationTimer;
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
    int lives = 5;
    int health = 100;
    bool canMoveRight;
    bool canMoveLeft;
    QString playerState = "base"; // base, super, fire
    Flag* finishFlag = nullptr;
    bool  winTriggered = false;
    bool isMovingLeft;
    bool isMovingRight;
    bool isJumping;
    bool isFacingRight;
    bool onGround;
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
