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
    pipe *pipe2 = new pipe(1100, 435, 2); // Example positions
    pipe *pipe3 = new pipe(1800, 435, 2);
    pipe *pipe4 = new pipe(2100, 435, 2);
    pipe *pipe5 = new pipe(2900, 435, 2);
    pipe *pipe6 = new pipe(3550, 435, 2);
    pipe *pipe7 = new pipe(4050, 435, 2);
    scene->addItem(pipe1);
    scene->addItem(pipe2);
    scene->addItem(pipe3);
    scene->addItem(pipe4);
    scene->addItem(pipe5);
    scene->addItem(pipe6);
    scene->addItem(pipe7);

    QList<pipe*> pipes;
    pipes.append(pipe1);
    pipes.append(pipe2);
    pipes.append(pipe3);
    pipes.append(pipe4);
    pipes.append(pipe5);
    pipes.append(pipe6);
    pipes.append(pipe7);




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
    Goomba *goomba6 = new Goomba(450, 2200);
    Goomba *goomba7 = new Goomba(450, 3150);
    Goomba *goomba8 = new Goomba(450, 3800);
    Goomba *goomba9 = new Goomba(450, 4150);

    goomba1->setPlatforms(platforms);
    goomba2->setPlatforms(platforms);
    goomba3->setPlatforms(platforms);
    goomba4->setPlatforms(platforms);
    goomba5->setPlatforms(platforms);
    goomba6->setPlatforms(platforms);
    goomba7->setPlatforms(platforms);
    goomba8->setPlatforms(platforms);
    goomba9->setPlatforms(platforms);


    goomba1->setPipes(pipes);
    goomba2->setPipes(pipes);
    goomba3->setPipes(pipes);
    goomba4->setPipes(pipes);
    goomba5->setPipes(pipes);
    goomba6->setPipes(pipes);
    goomba7->setPipes(pipes);
    goomba8->setPipes(pipes);
    goomba9->setPipes(pipes);



    scene->addItem(goomba1);
    scene->addItem(goomba2);
    scene->addItem(goomba3);
    scene->addItem(goomba4);
    scene->addItem(goomba5);
    scene->addItem(goomba6);
    scene->addItem(goomba7);
    scene->addItem(goomba8);
    scene->addItem(goomba9);


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

    QGraphicsTextItem* healthText = new QGraphicsTextItem();
    healthText->setPlainText("Heath: 100");
    healthText->setDefaultTextColor(Qt::white);
    healthText->setFont(QFont("Arial", 16));
    scene->addItem(healthText);

    healthText->setPos(10, 70);

    center = new QPointF;
    *center=mario->pos();

    QTimer* cameraTimer = new QTimer();
    connect(cameraTimer, &QTimer::timeout, this, [this, mario, scoreText, livesText, healthText]() {
        livesText->setPlainText("Lives: " + QString::number(mario->getLives()));
        scoreText->setPlainText("Score: " + QString::number(mario->getScore()));
        healthText->setPlainText("Heath: " + QString::number(mario->getHealth()));
        if(mario->pos().x()>center->x() || mario->pos().x() == 120){
            *center=mario->pos();
        }
        view->centerOn(*center);
        scoreText->setPos(view->mapToScene(10, 10));
        livesText->setPos(view->mapToScene(10, 40));
        healthText->setPos(view->mapToScene(10, 70));
    });
    cameraTimer->start(1);
}

Game::~Game()
{
    delete ui;
}
