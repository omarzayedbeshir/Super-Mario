#ifndef GAME_H
#define GAME_H
#include <QJsonArray>
#include <QMainWindow>
#include <QGraphicsView>

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
    void loadLevel(int levelNumber);
    void loadPlayerPosition(const QJsonObject &levelData);
    void loadPlatforms(const QJsonArray &platforms);
    void loadEnemies(const QJsonArray &enemies);

private:
    Ui::Game *ui;
    int current_level;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QPointF *center;
};
#endif // GAME_H
