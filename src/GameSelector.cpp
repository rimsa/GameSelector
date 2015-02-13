#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QMetaObject>

#include <GameSelector.h>
#include <Game.h>
#include "ui_GameSelector.h"

GameSelector::GameSelector(const QString& root, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameSelector),
    m_root(root),
    m_loader(m_root) {

    ui->setupUi(this);

    QObject::connect(&m_loader, SIGNAL(gameLoaded(GameInfo)), this, SLOT(createGame(GameInfo)));
    QObject::connect(&m_loader, SIGNAL(loadingFinished()), this, SLOT(show()), Qt::QueuedConnection);

    QMetaObject::invokeMethod(&m_loader, "load", Qt::QueuedConnection);
}

GameSelector::~GameSelector() {
    delete ui;
}

void GameSelector::createGame(GameInfo info) {
    Game* g = new Game(info);
    ui->gameList->addGame(g);
}
