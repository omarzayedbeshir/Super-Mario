#ifndef MARIO_H
#define MARIO_H

#include <QGraphicsRectItem>
#include <QKeyEvent>

class Mario : public QGraphicsRectItem
{
public:
    Mario(int x, int y);
private:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MARIO_H
