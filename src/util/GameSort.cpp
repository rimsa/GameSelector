#include <algorithm>

#include <GameSelector/util/GameComparator.h>
#include <GameSelector/util/GameSort.h>
#include <GameSelector/widgets/Game.h>

GameSort::GameSort(OrderType type, Qt::SortOrder order, QObject* parent) :
    QObject(parent),
    m_type(type),
    m_order(order) {
}

GameSort::~GameSort() {
}

void GameSort::setType(OrderType type) {
    if (m_type != type) {
        m_type = type;

        emit typeChanged(type);
    }
}

void GameSort::setOrder(Qt::SortOrder order) {
    if (m_order != order) {
        m_order = order;

        emit orderChanged(order);
    }
}

void GameSort::sort(QList<Game*>& games) {
    // Ordering using Quick Sort.
    std::sort(games.begin(), games.end(), GameComparator(m_type, m_order));
}
