#include "game.h"
#include "ui_game.h"
#include <QGraphicsScene>
#include "platform.h"
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QTimer>
#include "mario.h"
#include "goomba.h"
#include "pipe.h"
#include <QPixmap>
#include "flag.h"


Game::Game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Game)
{
    current_level = 1;
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 6000, 600);
    QPixmap backgroundImage(":graphics/Mario Game Assets/Background_2.png");
    backgroundImage = backgroundImage.scaled(3000, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    scene->setBackgroundBrush(backgroundImage);

    view = new QGraphicsView(this);
    view->setScene(scene);
    view->setFixedSize(800, 600);
    setCentralWidget(view);

    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    // Setting up platforms
    Platform *platform1 = new Platform(100, 500, 600, 30);
    Platform *platform2 = new Platform(800, 500, 600, 30);
    Platform *platform3 = new Platform(1500, 500, 1000, 30);
    Platform *platform4 = new Platform(2600, 500, 800, 30);
    Platform *platform5 = new Platform(3500, 500, 1000, 30);
    Platform *platform6 = new Platform(4550, 500, 500, 30);



    scene->addItem(platform1);
    scene->addItem(platform2);
    scene->addItem(platform3);
    scene->addItem(platform4);
    scene->addItem(platform5);
    scene->addItem(platform6);


    // Setting up pipes
    pipe *pipe1 = new pipe(400, 435, 2.0);
    scene->addItem(pipe1);
    pipe *pipe2 = new pipe(1800, 435, 2); // Example positions
    pipe *pipe3 = new pipe(1100, 435, 2);
    scene->addItem(pipe2);
    scene->addItem(pipe3);

    QList<pipe*> pipes;
    pipes.append(pipe1);
    pipes.append(pipe2);
    pipes.append(pipe3);



    QList<QGraphicsItem*> allItems = scene->items();
    QList<QGraphicsItem*> platforms;

    for (QGraphicsItem *item : allItems) {
        if (dynamic_cast<Platform*>(item)) {
            platforms.append(item);
    }   
    }

    // Setting up Mario
    Mario *mario = new Mario(450, 120, scene);
    mario->setPlatforms(platforms);
    mario->setPipes(pipes);
    scene->addItem(mario);
    mario->setFlag(QGraphicsItem::ItemIsFocusable);
    mario->setFocus();

    Flag* finishFlag = new Flag(4850, 180);
    scene->addItem(finishFlag);

    mario->setFinishFlag(finishFlag);


    setFixedSize(800, 600);

    // Setting up Enemies
    Goomba *goomba1 = new Goomba(450, 240);
    Goomba *goomba2 = new Goomba(450, 470);
    Goomba *goomba3 = new Goomba(450, 966);
    Goomba *goomba4 = new Goomba(450, 1600);
    Goomba *goomba5 = new Goomba(450, 1870);
    goomba1->setPlatforms(platforms);
    goomba2->setPlatforms(platforms);
    goomba3->setPlatforms(platforms);
    goomba4->setPlatforms(platforms);
    goomba5->setPlatforms(platforms);
    goomba1->setPipes(pipes);
    goomba2->setPipes(pipes);
    goomba3->setPipes(pipes);
    goomba4->setPipes(pipes);
    goomba5->setPipes(pipes);


    scene->addItem(goomba1);
    scene->addItem(goomba2);
    scene->addItem(goomba3);
    scene->addItem(goomba4);
    scene->addItem(goomba5);


    // Setting up statistics
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
    statsUpdater->start(1);
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
