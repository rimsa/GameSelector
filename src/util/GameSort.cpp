#include <QtAlgorithms>

#include <GameSelector/util/GameComparator.h>
#include <GameSelector/util/GameSort.h>
#include <GameSelector/widgets/Game.h>

GameSort::GameSort(OrderType type, QObject* parent) :
    QObject(parent),
    m_type(type) {
}

GameSort::~GameSort() {
}

void GameSort::setType(OrderType type) {
    if (m_type != type) {
        m_type = type;

        emit typeChanged(type);
    }
}

void GameSort::sort(QList<Game*>& games) {
    // Ordering using Quick Sort.
    qSort(games.begin(), games.end(), GameComparator(m_type));
}
