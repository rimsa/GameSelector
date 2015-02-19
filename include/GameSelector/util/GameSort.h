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
        ByYear,
        BySize,
    };
    friend class GameComparator;

    GameSort(OrderType type = ByName, Qt::SortOrder order = Qt::AscendingOrder, QObject* parent = 0);
    virtual ~GameSort();

    OrderType type() const { return m_type; }
    void setType(OrderType type);

    Qt::SortOrder order() const { return m_order; }
    void setOrder(Qt::SortOrder order);

public slots:
    void sort(QList<Game*>& games);

signals:
    void typeChanged(GameSort::OrderType type);
    void orderChanged(Qt::SortOrder order);

private:
    OrderType m_type;
    Qt::SortOrder m_order;

};

Q_DECLARE_METATYPE(GameSort::OrderType)

#endif // GAMESORT_H
