#ifndef GAME_H
#define GAME_H
#include <QJsonArray>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "castle.h"
#include "mario.h"
#include "platform.h"
#include "pipe.h"
#include "flag.h"
#include <QPair>

QT_BEGIN_NAMESPACE
namespace Ui {
class Game;
}
QT_END_NAMESPACE

class Game : public QMainWindow
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);
    ~Game();
    void renderLevel(int levelNumber);
    void UISetup();
private:
    Ui::Game *ui;
    int current_level;
    QGraphicsScene *scene;
    QGraphicsView *view;
    Mario* mario;
    QPointF *center;
    QList<Platform*> platformsList;
    QList<pipe*> pipesList;
    Flag* finishFlag;
    QGraphicsTextItem* scoreText;
    QGraphicsTextItem* livesText;
    QGraphicsTextItem* healthText;
    QGraphicsTextItem* levelText;
    QGraphicsTextItem* funnyText;
};
#endif // GAME_H
