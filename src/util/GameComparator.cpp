#include <GameSelector/util/GameComparator.h>
#include <GameSelector/widgets/Game.h>

GameComparator::GameComparator(GameSort::OrderType type) :
    m_type(type) {
}

GameComparator::~GameComparator() {
}

bool GameComparator::operator()(const Game* a, const Game* b) const {
    switch (m_type) {
        case GameSort::ByName:
            return a->name() < b->name();
        case GameSort::ByGenre:
            return a->genre() < b->genre();
        case GameSort::ByDeveloper:
            return a->developer() < b->developer();
        case GameSort::ByPublisher:
            return a->publisher() < b->publisher();
        case GameSort::ByYear:
            return a->year() < b->year();
        case GameSort::BySize:
            return a->size() < b->size();
        default:
            Q_ASSERT(!"Invalid comparator type");
            return false;
    }
}
