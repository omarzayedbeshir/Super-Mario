QT       += core gui
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boss.cpp \
    coin.cpp \
    flag.cpp \
    goomba.cpp \
    koopatroopa.cpp \
    main.cpp \
    game.cpp \
    mario.cpp \
    mushroom.cpp \
    paratroopa.cpp \
    pipe.cpp \
    piranha.cpp \
    platform.cpp \
    star.cpp

HEADERS += \
    boss.h \
    coin.h \
    flag.h \
    game.h \
    goomba.h \
    koopatroopa.h \
    mario.h \
    mushroom.h \
    paratroopa.h \
    pipe.h \
    piranha.h \
    platform.h \
    star.h

FORMS += \
    game.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
