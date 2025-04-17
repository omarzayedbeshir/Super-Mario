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

class pipe;
class Mario : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Mario(int x, int y, QGraphicsScene* scene);
    void setPlatforms(const QList<QGraphicsItem*>& platforms);
    int getLives();
    int getScore();
    bool collidesHorizontally(double dx);
    void setPipes(const QList<pipe*>& pipes);
    void isCollidingWithPipes();






private:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void isCollidingWithDynamicObstacles();
    void canTakeDamageTruthify();
    void checkHorizontalCollisions();
    QGraphicsScene* currentScene;
    QList<QGraphicsItem*> platformList;
    QList<QGraphicsItem*> dynamicObstaclesList;
    QList<pipe*> pipeList;
    QTimer* gravityTimer;
    QTimer* dynamicObstaclesTimer;
    QTimer* damageCoolDownTimer;
    double velocityY = 0;
    double gravity = 0.4;
    double scale = 2.0;
    int score = 0;
    int height = 16 * scale;
    bool onGround = false;
    bool canTakeDamage = true;
    int lives = 10;
    QSet<int> pressedKeys;
private slots:
    void applyGravity();
};

#endif // MARIO_H
