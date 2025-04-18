#include "flag.h"
#include <QPixmap>

Flag::Flag(qreal x, qreal y, QGraphicsItem* parent)
    : QObject(), QGraphicsItemGroup(parent)
{
    // Pole
    poleItem = new QGraphicsPixmapItem(QPixmap(":/graphics/Mario Game Assets/FlagPole.png"));
    poleItem->setScale(2.0);
    poleItem->setPos(x, y);
    addToGroup(poleItem);

    flagItem = new QGraphicsPixmapItem(QPixmap(":/graphics/Mario Game Assets/Flag.png"));
    flagItem->setScale(3.0);

    flagItem->setPos(x - 30, y + 30);
    addToGroup(flagItem);

    slideTimer = new QTimer(this);
    connect(slideTimer, &QTimer::timeout, this, &Flag::slideStep);
}

QGraphicsPixmapItem* Flag::getFlag() const {
    return poleItem;
}

void Flag::startFlagAnimation() {
    if (animating) return;
    animating = true;
    slideTimer->start(16);
}

void Flag::slideStep() {
    if (movedDistance < slideTarget) {
        flagItem->moveBy(0, slideStepSize);
        movedDistance += slideStepSize;
        emit sliding(slideStepSize);
    } else {
        slideTimer->stop();
        emit animationFinished();
    }
}

