#include "game.h"
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
    setFixedSize(800, 600);

    view = new QGraphicsView(this);
    view->setScene(scene);
    view->setFixedSize(800, 600);
    setCentralWidget(view);

    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Setting up Mario
    Mario *mario = renderLevel(current_level, scene);

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

Mario* Game::renderLevel(int levelNumber, QGraphicsScene* scene) {
    qDeleteAll(platformsList);
    qDeleteAll(pipesList);
    pipesList.clear();
    platformsList.clear();
    pipesList.clear();

    QFile file(":/graphics/Mario Game Assets/levels/level" + QString::number(levelNumber) + ".json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open level file";
        return nullptr;
    }

    scene->clear();
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
        /*
        { "x": 450, "y": 3800 }
        */
    }

    for (auto val : root["koopa_troopas"].toArray()) {
        auto obj = val.toObject();
        KoopaTroopa* koopa = new KoopaTroopa(obj["x"].toInt(), obj["y"].toInt());
        koopa->setPlatforms(platformsList);
        koopa->setPipes(pipesList);
        scene->addItem(koopa);
    }

    for (auto val : root["paratroopas"].toArray()) {
        auto obj = val.toObject();
        Paratroopa* paratroopa = new Paratroopa(obj["x"].toInt(), obj["y"].toInt());
        paratroopa->setPlatforms(platformsList);
        paratroopa->setPipes(pipesList);
        scene->addItem(paratroopa);
    }


    if (root.contains("flag")) {
        auto obj = root["flag"].toObject();
        Flag* flag = new Flag(obj["x"].toInt(), obj["y"].toInt());
        scene->addItem(flag);
        finishFlag = flag;
    }

    Mario* mario;

    if (root.contains("mario")) {
        auto obj = root["mario"].toObject();
        mario = new Mario(obj["x"].toInt(), obj["y"].toInt(), scene);
        mario->setPipes(pipesList);
        mario->setFinishFlag(finishFlag);
        mario->setFlag(QGraphicsItem::ItemIsFocusable);
        mario->setFocus();
        scene->addItem(mario);
    }

    return mario;
}
