#ifndef MARIO_H
#define MARIO_H

#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsView>
#include <QList>

class Mario : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Mario(int x, int y);
    void setPlatforms(const QList<QGraphicsItem*>& platforms);

private:
    void keyPressEvent(QKeyEvent *event);
    QList<QGraphicsItem*> platformList;
    QTimer* gravityTimer;
    float velocityY = 0;
    float gravity = 0.4;
    bool onGround = false;
private slots:
    void applyGravity();
};

#endif // MARIO_H
