#ifndef DYNAMICOBSTACLE_H
#define DYNAMICOBSTACLE_H
#include <QObject>
#include <QGraphicsPixmapItem>

class DynamicObstacle: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    DynamicObstacle();
private slots:
    virtual void move() = 0;
};

#endif // DYNAMICOBSTACLE_H
