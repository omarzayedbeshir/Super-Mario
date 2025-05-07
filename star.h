#ifndef STAR_H
#define STAR_H
#include <QObject>
#include <QGraphicsPixmapItem>

class Star : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Star(int x, int y);

private:
    int scale = 2;
};
#endif // STAR_H

