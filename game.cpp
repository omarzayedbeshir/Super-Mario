#include "game.h"
#include "piranha.h"
#include "ui_game.h"
#include <QGraphicsScene>
#include "platform.h"
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QTimer>
#include "mario.h"
#include "goomba.h"
#include "mushroom.h"
#include "koopatroopa.h"
#include <QPushButton>
#include "pipe.h"
#include <QPixmap>
#include "flag.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "star.h"
#include <QElapsedTimer>
#include "paratroopa.h"
#include "boss.h"
#include "coin.h"

Game::Game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);
    setFixedSize(800, 600);

    view = new QGraphicsView(this);
    view->setFixedSize(800, 600);
    setCentralWidget(view);
    setWindowTitle("Super Mario");
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);
    QPixmap backgroundImage(":graphics/Mario Game Assets/Background_2.png");
    backgroundImage = backgroundImage.scaled(3000, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    scene->setBackgroundBrush(backgroundImage);
    QGraphicsTextItem* title = new QGraphicsTextItem("Super Mario");
    title->setDefaultTextColor(Qt::white);
    title->setFont(QFont("Arial", 36, QFont::Bold));
    title->setPos(250, 100);
    scene->addItem(title);

    QPushButton* startButton = new QPushButton("Start Game");
    startButton->setFixedSize(200, 50);
    startButton->move(300, 300);
    startButton->setStyleSheet("background-color: green; color: white; font-size: 18px;");

    // Add button to the view
    view->setScene(scene);
    view->scene()->addWidget(startButton);

    // Connect button to start the game
    connect(startButton, &QPushButton::clicked, this, [=]() {
        UISetup();
        renderLevel(1);
    });
}

void Game::UISetup() {
    mario = new Mario;
    center = new QPointF;

    scoreText = new QGraphicsTextItem();
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("Arial", 16));

    livesText = new QGraphicsTextItem();
    livesText->setDefaultTextColor(Qt::white);
    livesText->setFont(QFont("Arial", 16));

    healthText = new QGraphicsTextItem();
    healthText->setDefaultTextColor(Qt::white);
    healthText->setFont(QFont("Arial", 16));

    levelText = new QGraphicsTextItem();
    levelText->setDefaultTextColor(Qt::white);
    levelText->setFont(QFont("Arial", 16));

    QTimer* cameraTimer = new QTimer();
    connect(cameraTimer, &QTimer::timeout, this, [this]() {
        livesText->setPlainText("Lives: " + QString::number(mario->getLives()));
        scoreText->setPlainText("Score: " + QString::number(mario->getScore()));
        healthText->setPlainText("Health: " + QString::number(mario->getHealth()));
        levelText->setPlainText("Level: " + QString::number(mario->getLevel()));
        if(mario->pos().x() == 0){
            renderLevel(mario->getLevel());
        }
        if(mario->pos().x()>center->x()){
            *center=mario->pos();
        }
        view->centerOn(*center);
        scoreText->setPos(view->mapToScene(10, 10));
        livesText->setPos(view->mapToScene(10, 40));
        healthText->setPos(view->mapToScene(10, 70));
        levelText->setPos(view->mapToScene(10, 100));
    });
    cameraTimer->start(1);
}

Game::~Game()
{
    delete ui;
}

void Game::renderLevel(int levelNumber) {
    platformsList.clear();
    pipesList.clear();
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 6000, 600);
    QPixmap backgroundImage(":graphics/Mario Game Assets/Background_2.png");
    backgroundImage = backgroundImage.scaled(3000, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    scene->setBackgroundBrush(backgroundImage);
    view->setScene(scene);

    scene->addItem(scoreText);
    scoreText->setPos(10, 10);

    scene->addItem(livesText);
    livesText->setPos(10, 40);

    scene->addItem(healthText);
    healthText->setPos(10, 70);

    scene->addItem(levelText);
    healthText->setPos(10, 100);

    QFile file(":/graphics/Mario Game Assets/levels/level" + QString::number(levelNumber) + ".json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open level file";
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();

    for (auto val : root["platforms"].toArray()) {
        auto obj = val.toObject();
        Platform* platform = new Platform(obj["x"].toInt(), obj["y"].toInt(),
                                          obj["width"].toInt(), obj["height"].toInt());
        scene->addItem(platform);
        platformsList.append(platform);
    }

    for (auto val : root["pipes"].toArray()) {
        auto obj = val.toObject();
        pipe* p = new pipe(obj["x"].toInt(), obj["y"].toInt(), obj["scale"].toDouble());
        scene->addItem(p);
        pipesList.append(p);
    }

    for (auto val : root["piranhas"].toArray()) {
        auto obj = val.toObject();
        pipe* homePipe = nullptr;
        for (pipe* p : pipesList) {
                homePipe = p;
            }
        Piranha* piranha = new Piranha(obj["x"].toInt(), obj["y"].toInt(), homePipe);
        scene->addItem(piranha);
    }


    for (auto val : root["coins"].toArray()) {
        auto obj = val.toObject();
        Coin* coin = new Coin(obj["x"].toInt(), obj["y"].toInt());
        scene->addItem(coin);
    }

    for (auto val : root["mushrooms"].toArray()) {
        auto obj = val.toObject();
        Mushroom* mushroom = new Mushroom(obj["x"].toInt(), obj["y"].toInt());
        scene->addItem(mushroom);
    }

    for (auto val : root["stars"].toArray()) {
        auto obj = val.toObject();
        Star* star = new Star(obj["x"].toInt(), obj["y"].toInt());
        scene->addItem(star);
    }

    for (auto val : root["goombas"].toArray()) {
        auto obj = val.toObject();
        Goomba* goomba = new Goomba(obj["x"].toInt(), obj["y"].toInt());
        goomba->setPlatforms(platformsList);
        goomba->setPipes(pipesList);
        scene->addItem(goomba);
    }

    for (auto val : root["paratroopas"].toArray()) {
        auto obj = val.toObject();
        Paratroopa* paratroopa = new Paratroopa(obj["x"].toInt(), obj["y"].toInt());
        paratroopa->setPlatforms(platformsList);
        paratroopa->setPipes(pipesList);
        scene->addItem(paratroopa);
    }

    for (auto val : root["koopa_troopas"].toArray()) {
        auto obj = val.toObject();
        KoopaTroopa* koopa = new KoopaTroopa(obj["x"].toInt(), obj["y"].toInt());
        koopa->setPlatforms(platformsList);
        koopa->setPipes(pipesList);
        scene->addItem(koopa);
    }

    if (root.contains("flag")) {
        auto obj = root["flag"].toObject();
        Flag* flag = new Flag(obj["x"].toInt(), obj["y"].toInt());
        scene->addItem(flag);
        finishFlag = flag;
    }

    if (root.contains("mario")) {
        auto obj = root["mario"].toObject();
        mario->setScene(scene);
        mario->setInitPos(obj["x"].toInt(), obj["y"].toInt());
        mario->setPipes(pipesList);
        mario->setFinishFlag(finishFlag);
        mario->setFlag(QGraphicsItem::ItemIsFocusable);
        if(levelNumber == 1){
            mario->setScore(0);
            mario->setHealth(100);
        }
        scene->addItem(mario);
        mario->setFocus();
        *center=mario->pos();
    }

    if (root.contains("boss")) {
        auto obj = root["boss"].toObject();
        Boss* boss = new Boss(obj["x"].toInt(), obj["y"].toInt(), scene);
        scene->addItem(boss);
    }

    if (root.contains("funny_text")) {
        QGraphicsTextItem *funnyText = new QGraphicsTextItem();
        funnyText->setPlainText(root["funny_text"].toString());
        funnyText->setDefaultTextColor(Qt::white);
        funnyText->setFont(QFont("Arial", 16));
        scene->addItem(funnyText);
        funnyText->setPos(100, 300);
    }
}
