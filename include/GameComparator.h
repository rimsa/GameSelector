#ifndef GAMECOMPARATOR_H
#define GAMECOMPARATOR_H

#include <GameSort.h>

class Game;

class GameComparator {
public:
    GameComparator(GameSort::OrderType type);
    virtual ~GameComparator();

    bool operator()(const Game* a, const Game* b) const;

private:
    GameSort::OrderType m_type;

};

#endif // GAMECOMPARATOR_H
