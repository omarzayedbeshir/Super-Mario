#include "coin.h"

Coin::Coin(int x, int y) {
    setPos(x, y);
    setScale(scale);
    setPixmap(QPixmap(":graphics/Mario Game Assets/Coin.png"));
}
