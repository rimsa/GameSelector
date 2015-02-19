#ifndef GAMECOMPARATOR_H
#define GAMECOMPARATOR_H

#include <GameSelector/util/GameSort.h>

class Game;

class GameComparator {
public:
    GameComparator(GameSort::OrderType type, Qt::SortOrder order);
    virtual ~GameComparator();

    bool operator()(const Game* a, const Game* b) const;

private:
    GameSort::OrderType m_type;
    Qt::SortOrder m_order;

};

#endif // GAMECOMPARATOR_H
