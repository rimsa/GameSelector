#include <QDebug>
#include <QLayout>

#include <Game.h>
#include <GameList.h>

GameList::GameList(QWidget* parent) :
    QWidget(parent) {
}

GameList::~GameList() {
}

void GameList::addGame(Game* g) {
    m_games.push_back(g);
    this->layout()->addWidget(g);
}
