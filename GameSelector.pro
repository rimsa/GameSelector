#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T12:25:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameSelector
TEMPLATE = app

INCLUDEPATH += include

SOURCES += src/main.cpp \
        src/GameSelector.cpp \
        src/GameLoader.cpp

HEADERS  += include/GameSelector.h \
    include/GameInfo.h \
    include/GameLoader.h

FORMS    += forms/GameSelector.ui

RESOURCES += \
    rccs/gameselector.qrc
