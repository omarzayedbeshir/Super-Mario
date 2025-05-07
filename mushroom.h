#ifndef MUSHROOM_H
#define MUSHROOM_H
#include <QObject>
#include <QGraphicsPixmapItem>

class Mushroom : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Mushroom(int x, int y);
private:
    double scale = 2.0;
    int height = 16 * scale;
};

#endif // MUSHROOM_H
