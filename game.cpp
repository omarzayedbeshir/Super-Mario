#include "game.h"
#include "ui_game.h"
#include <QGraphicsScene>
#include "platform.h"
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QTimer>
#include "mario.h"
#include "goomba.h"

Game::Game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Game)
{
    current_level = 1;
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 3000, 600);

    view = new QGraphicsView(this);
    view->setScene(scene);
    view->setFixedSize(800, 600);
    setCentralWidget(view);

    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    Platform *platform = new Platform(100, 500, 600, 30);
    Mario *mario = new Mario(450, 120, scene);
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

    QGraphicsTextItem* scoreText = new QGraphicsTextItem();
    scoreText->setPlainText("Score: 0");
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("Arial", 16));
    scene->addItem(scoreText);

    scoreText->setPos(10, 10);

    QGraphicsTextItem* livesText = new QGraphicsTextItem();
    livesText->setPlainText("Lives: 10");
    livesText->setDefaultTextColor(Qt::white);
    livesText->setFont(QFont("Arial", 16));
    scene->addItem(livesText);

    livesText->setPos(10, 40);

    QTimer* statsUpdater = new QTimer();
    connect(statsUpdater, &QTimer::timeout, this, [this, mario, scoreText, livesText]() {
        livesText->setPlainText("Lives: " + QString::number(mario->getLives()));
        scoreText->setPlainText("Score: " + QString::number(mario->getScore()));

        scoreText->setPos(view->mapToScene(10, 10));
        livesText->setPos(view->mapToScene(10, 40));
    });
    statsUpdater->start(16);
    view->centerOn(mario);

    QTimer* cameraTimer = new QTimer();
    connect(cameraTimer, &QTimer::timeout, this, [this, mario]() {
        view->centerOn(mario);
    });
    cameraTimer->start(1);
}

Game::~Game()
{
    delete ui;
}
