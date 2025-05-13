#ifndef COIN_H
#define COIN_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Coin : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Coin(int x, int y);
private:
    double scale = 2.0;
    int height = 16 * scale;
};

#endif // COIN_H
