#ifndef GAMESORT_H
#define GAMESORT_H

#include <QObject>

class Game;
class GameComparator;

class GameSort : public QObject {
    Q_OBJECT
    Q_ENUMS(OrderType)

public:
    enum OrderType {
        ByName,
        ByGenre,
        ByDeveloper,
        ByPublisher,
        ByYear
    };
    friend class GameComparator;

    GameSort(OrderType type = ByName, QObject* parent = 0);
    virtual ~GameSort();

    OrderType type() const { return m_type; }
    void setType(OrderType type);

public slots:
    void sort(QList<Game*> games);

signals:
    void typeChanged(GameSort::OrderType type);

private:
    OrderType m_type;

};

Q_DECLARE_METATYPE(GameSort::OrderType)

#endif // GAMESORT_H
