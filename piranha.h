#ifndef PIRANHA_H
#define PIRANHA_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include "pipe.h"

class Piranha : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Piranha(int x, int y, pipe* homePipe);
    void setPipes(const QList<pipe*>& pipes);
    int getHeight() const;
    int getdownY() const;

private slots:
    void updatePosition();
    void updateAnimation();

private:
    QTimer* positionTimer;
    QTimer* animationTimer;
    pipe* home;
    bool isUp = false;
    int frame = 0;
    int upY;
    int downY;
    int height = 32;
    const int speed = 2;
    const int pauseTime = 60;
    int pauseCounter = 0;
};

#endif // PIRANHA_H
