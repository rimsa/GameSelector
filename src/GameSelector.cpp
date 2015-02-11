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
    QObject::connect(&m_loader, SIGNAL(loadingFinished()), this, SLOT(show()));

    m_loader.load();
}

GameSelector::~GameSelector() {
    delete ui;
}

void GameSelector::createGame(GameInfo info) {
    qDebug() << "Game: ";
    qDebug() << "  Name: " << info.name;
    qDebug() << "  DiskName: " << info.diskName;
    qDebug() << "  Cover: " << info.cover;
    qDebug() << "  Genre: " << info.genre;
    qDebug() << "  Developer: " << info.developer;
    qDebug() << "  Publisher: " << info.publisher;
    qDebug() << "  Year: " << info.year;


    ui->widget->setPixmap(info.cover);
    this->repaint();
}
