QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameSelector
TEMPLATE = app

INCLUDEPATH += include

SOURCES += src/main.cpp \
    src/GameSelector.cpp \
    src/GameLoader.cpp \
    src/Game.cpp \
    src/GameList.cpp \
    src/GameFilter.cpp \
    src/GameSort.cpp \
    src/GameComparator.cpp

HEADERS  += include/GameSelector.h \
    include/GameInfo.h \
    include/GameLoader.h \
    include/Game.h \
    include/GameList.h \
    include/GameFilter.h \
    include/GameSort.h \
    include/GameComparator.h

FORMS    += forms/GameSelector.ui

RESOURCES += \
    rccs/gameselector.qrc
