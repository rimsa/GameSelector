QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameSelector
TEMPLATE = app

INCLUDEPATH += include

SOURCES += \
    src/main.cpp \
    src/GameSelector.cpp \
    src/loader/GameLoader.cpp \
    src/util/GameFilter.cpp \
    src/util/GameSort.cpp \
    src/util/GameComparator.cpp \
    src/widgets/Game.cpp \
    src/widgets/GameScroller.cpp

HEADERS  += \
    include/GameSelector/GameSelector.h \
    include/GameSelector/loader/GameLoader.h \
    include/GameSelector/util/GameInfo.h \
    include/GameSelector/util/GameFilter.h \
    include/GameSelector/util/GameSort.h \
    include/GameSelector/util/GameComparator.h \
    include/GameSelector/widgets/Game.h \
    include/GameSelector/widgets/GameScroller.h

FORMS    += forms/GameSelector.ui

RESOURCES += \
    rccs/GameSelector.qrc
