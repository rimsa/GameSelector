#include <QDebug>
#include <QLayout>
#include <QtAlgorithms>

#include <Game.h>
#include <GameList.h>
#include <GameComparator.h>

GameList::GameList(QWidget* parent) :
    QWidget(parent),
    m_updated(true),
    m_order(GameComparator::ByName) {
}

GameList::~GameList() {
}

void GameList::addGame(Game* g) {
    m_games.push_back(g);

    m_updated = false;
    this->update();
}

void GameList::removeGame(Game* g) {
    m_games.removeAll(g);

    m_updated = false;
    this->updateGames();
}

void GameList::removeAllGames() {
    m_games.clear();

    m_updated = false;
    this->updateGames();
}

void GameList::orderBy(GameComparator::OrderType type) {
    if (m_order != type) {
        m_order = type;

        m_updated = false;
        this->updateGames();
    }
}

void GameList::showEvent(QShowEvent* event) {
    Q_UNUSED(event);

    this->updateGames();
}

void GameList::updateGames() {
    if (m_updated)
        return;

    if (!this->isVisible())
        return;

    // Remover todos os jogos.
    QLayoutItem* item;
    while ((item = this->layout()->takeAt(0)) != 0) {
        if (qobject_cast<Game*>(item->widget()))
            delete item;
    }

    // Ordenar todos os jogos.
    qSort(m_games.begin(), m_games.end(), GameComparator(m_order));

    // Colocar todos os jogos de volta no layout.
    foreach (Game* g, m_games)
        this->layout()->addWidget(g);

    // Atualizar o estado informando que está atualizado.
    m_updated = true;
}
