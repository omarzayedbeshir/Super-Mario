#include "mario.h"

Mario::Mario(int x, int y) {
    setRect(0, 0, 50, 50);
    setPos(y, x);
}

void Mario::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        setPos(x() - 10, y());
        break;
    case Qt::Key_Right:
        setPos(x() + 10, y());
        break;
    case Qt::Key_Up:
        setPos(x(), y() - 10);
        break;
    case Qt::Key_Space:
        setPos(x(), y() - 10);
        break;
    }
}
