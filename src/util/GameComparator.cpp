#include <GameSelector/util/GameComparator.h>
#include <GameSelector/widgets/Game.h>

GameComparator::GameComparator(GameSort::OrderType type, Qt::SortOrder order) :
    m_type(type),
    m_order(order) {
}

GameComparator::~GameComparator() {
}

bool GameComparator::operator()(const Game* a, const Game* b) const {
    bool ret = false;

    switch (m_type) {
        case GameSort::ByName:
            ret = a->name() < b->name();
            break;
        case GameSort::ByGenre:
            ret = a->genre() < b->genre();
            break;
        case GameSort::ByDeveloper:
            ret = a->developer() < b->developer();
            break;
        case GameSort::ByPublisher:
            ret = a->publisher() < b->publisher();
            break;
        case GameSort::ByYear:
            ret = a->year() < b->year();
            break;
        case GameSort::BySize:
            ret = a->size() < b->size();
            break;
        default:
            Q_ASSERT(!"Invalid comparator type");
            return false;
    }

    return (m_order == Qt::AscendingOrder ? ret : !ret);
}
