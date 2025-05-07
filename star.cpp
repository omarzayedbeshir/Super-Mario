#include "star.h"

Star::Star(int x, int y) {
    setPos(x, y);
    setScale(scale);
    setPixmap(QPixmap(":/graphics/Mario Game Assets/Starman.png"));
}
