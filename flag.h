#ifndef FLAG_H
#define FLAG_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QTimer>


class Flag : public QObject, public QGraphicsItemGroup {
    Q_OBJECT

public:
    explicit Flag(qreal x, qreal y, QGraphicsItem* parent = nullptr);
    QGraphicsPixmapItem* getFlag() const;


public slots:
    /// Call this when Mario first touches the flag
    void startFlagAnimation();

signals:
    /// Emitted once the flag has slid all the way down
    void animationFinished();
    void sliding(int dy);


private slots:
    /// One step of the slide‑down
    void slideStep();
private:
    QGraphicsPixmapItem* poleItem;
    QGraphicsPixmapItem* flagItem;
    QTimer* slideTimer;
    int movedDistance = 0;
    const int slideTarget   = 200;
    const int slideStepSize =   5;
    bool animating = false;
};

#endif // FLAG_H
