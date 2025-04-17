#include "game.h"
#include "ui_game.h"
#include <QGraphicsScene>
#include "platform.h"
#include <QGraphicsView>
#include "mario.h"
#include "goomba.h"

Game::Game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Game)
{
    current_level = 1;
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    view = new QGraphicsView(this);
    view->setScene(scene);
    view->setFixedSize(800, 600);
    setCentralWidget(view);

    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    Platform *platform = new Platform(100, 500, 600, 30);
    Mario *mario = new Mario(450, 120);
    setFixedSize(800, 600);
    scene->addItem(platform);
    scene->addItem(mario);
    mario->setFlag(QGraphicsItem::ItemIsFocusable);
    mario->setFocus();

    Goomba *goomba1 = new Goomba(450, 240);
    scene->addItem(goomba1);

    QList<QGraphicsItem*> allItems = scene->items();
    QList<QGraphicsItem*> platforms;
    for (QGraphicsItem *item : allItems) {
        if (dynamic_cast<Platform*>(item)) {
            platforms.append(item);
        }
    }

    mario->setPlatforms(platforms);
    goomba1->setPlatforms(platforms);
}

Game::~Game()
{
    delete ui;
}
