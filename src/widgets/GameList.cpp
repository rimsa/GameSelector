#include <QDebug>

#include <QLayout>

#include <GameSelector/widgets/Game.h>
#include <GameSelector/widgets/GameList.h>

GameList::GameList(QWidget* parent) :
    QWidget(parent),
    m_updated(true),
    m_filter(this),
    m_sort(GameSort::ByName, this) {

    QObject::connect(&m_filter, SIGNAL(filterChanged()), this, SLOT(markUpdate()));
    QObject::connect(&m_sort, SIGNAL(typeChanged(GameSort::OrderType)), this, SLOT(markUpdate()));
}

GameList::~GameList() {
}

void GameList::addGame(Game* g) {
    m_games.push_back(g);

    this->markUpdate();
}

void GameList::addGames(QList<Game*> games) {
    m_games.append(games);

    this->markUpdate();
}

void GameList::removeGame(Game* g) {
    m_games.removeAll(g);

    this->markUpdate();
}

void GameList::removeAllGames() {
    m_games.clear();

    this->markUpdate();
}

void GameList::showEvent(QShowEvent* event) {
    Q_UNUSED(event);

    this->updateGames();
}

void GameList::markUpdate() {
    m_updated = false;

    QMetaObject::invokeMethod(this, "updateGames", Qt::QueuedConnection);
}

void GameList::updateGames() {
    // If already updated, no need to recompute.
    if (m_updated)
        return;

    // Only compute list if visible,
    // otherwise postpone it until
    // the widget is visible.
    if (!this->isVisible())
        return;

    // Remove all games from the layout.
    QLayoutItem* item;
    while ((item = this->layout()->takeAt(0)) != 0) {
        if (qobject_cast<Game*>(item->widget()))
            delete item;
    }

    // Build a new list with all the games.
    QList<Game*> displayGames(m_games);

    // Filter the games by the selected criterias.
    m_filter.applyFilter(displayGames);

    // Sort the games according to witch criteria.
    m_sort.sort(displayGames);

    // From this new list, put the games
    // back to the layout.
    foreach (Game* g, displayGames)
        this->layout()->addWidget(g);

    // Mark the state as updated so no
    // unnecessary computations are performed.
    m_updated = true;
}
