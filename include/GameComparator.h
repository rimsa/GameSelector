#ifndef GAMECOMPARATOR_H
#define GAMECOMPARATOR_H

class Game;

class GameComparator {
public:
    enum OrderType {
        ByName,
        ByGenre,
        ByDeveloper,
        ByPublisher,
        ByYear
    };

    GameComparator(OrderType type = ByName);
    virtual ~GameComparator();

    OrderType type() const { return m_type; }
    void setType(OrderType type) { m_type = type; }

    bool operator()(const Game* a, const Game* b) const;

private:
    OrderType m_type;

};

#endif // GAMECOMPARATOR_H
