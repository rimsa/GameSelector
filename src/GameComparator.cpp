#include <Game.h>
#include <GameComparator.h>

GameComparator::GameComparator(OrderType type) :
    m_type(type) {
}

GameComparator::~GameComparator() {
}

bool GameComparator::operator()(const Game* a, const Game* b) const {
    switch (m_type) {
        case GameComparator::ByName:
            return a->name() < b->name();
        case GameComparator::ByGenre:
            return a->genre() < b->genre();
        case GameComparator::ByDeveloper:
            return a->developer() < b->developer();
        case GameComparator::ByPublisher:
            return a->publisher() < b->publisher();
        case GameComparator::ByYear:
            return a->year() < b->year();
        default:
            Q_ASSERT(!"Invalid comparator type");
            return false;
    }
}
