#include "mushroom.h"
#include <QPixmap>

Mushroom::Mushroom(int x, int y) {
    setPos(x, y);
    setScale(scale);
    setPixmap(QPixmap(":graphics/Mario Game Assets/MagicMushroom.png"));
}
