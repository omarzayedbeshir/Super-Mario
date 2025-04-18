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

class Mario : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Mario(int x, int y, QGraphicsScene* scene);
    void setPlatforms(const QList<QGraphicsItem*>& platforms);
    int getLives();
    int getScore();
    void setPipes(const QList<pipe*>& pipes);
    void setFinishFlag(Flag* flag);        // <— add this

private:
    void handleSideCollision(const QRectF& marioRect, const QRectF& obstacleRect);
    void preventApproaching(const QRectF& marioRect, const QRectF& obstacleRect);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void isCollidingWithDynamicObstacles();
    void canTakeDamageTruthify();
    void isCollidingWithPipes();
    void updateAnimation();


    QSoundEffect* jumpSound;
    QSoundEffect* goombaHitSound;
    QGraphicsScene* currentScene;
    QList<QGraphicsItem*> platformList;
    QList<QGraphicsItem*> dynamicObstaclesList;
    QList<pipe*> pipeList;
    QTimer* gravityTimer;
    QTimer* dynamicObstaclesTimer;
    QTimer* damageCoolDownTimer;
    QTimer* runAnimationTimer;
    int currentRunFrame = 0;
    double velocityY = 0;
    double gravity = 0.4;
    double scale = 2.0;
    int score = 0;
    int height = 16 * scale;
    bool onGround = false;
    bool canTakeDamage = true;
    int lives = 10;
    bool canMoveRight;
    bool canMoveLeft;
    Flag* finishFlag = nullptr;
    bool  winTriggered = false;

    QSet<int> pressedKeys;
    QString horizontalDirection = "Right";
private slots:
    void applyGravity();
    void checkFlagCollision();     // ← new
    void onFlagSliding(int dy);     // new


};

#endif // MARIO_H
