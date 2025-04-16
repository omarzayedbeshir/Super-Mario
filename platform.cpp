#include "platform.h"
#include <QBrush>

Platform::Platform(int x, int y, int width, int height)
{
    setRect(0, 0, width, height);
    setPos(x, y);
    setBrush(QBrush(QColor(141, 79, 58)));
}
