#include "piranha.h"
#include <QGraphicsScene>
#include <QPixmap>

Piranha::Piranha(int x, int y, pipe* homePipe)
    : home(homePipe)
{
    setPixmap(QPixmap(":/graphics/Mario Game Assets/Piranha_1.png"));
    setScale(2);

    int pipeWidth = homePipe->boundingRect().width();


    downY = y + homePipe->getTopPart()->boundingRect().height() - height + 18;
    upY   = downY - height - 10;

    setPos(x+(pipeWidth/2), downY);

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &Piranha::updateAnimation);
    animationTimer->start(200);

    positionTimer = new QTimer(this);
    connect(positionTimer, &QTimer::timeout, this, &Piranha::updatePosition);
    positionTimer->start(16);
}

void Piranha::updateAnimation() {
    frame = (frame + 1) % 2;
    QString path = ":/graphics/Mario Game Assets/Piranha_" + QString::number(frame + 1) + ".png";
    setPixmap(QPixmap(path));
}

void Piranha::updatePosition() {
    if (pauseCounter > 0) {
        --pauseCounter;
        return;
    }

    if (!isUp) {
        setY(y() - speed);
        if (y() <= upY) {
            isUp = true;
            pauseCounter = pauseTime;
        }
    } else {
        setY(y() + speed);
        if (y() >= downY) {
            isUp = false;
            pauseCounter = pauseTime;
        }
    }
}


int Piranha::getHeight() const {
    return height;
}

int Piranha::getdownY() const {
    return downY;
}
