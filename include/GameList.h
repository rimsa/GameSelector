#ifndef GAMELIST_H
#define GAMELIST_H

#include <QWidget>
#include <GameComparator.h>

class Game;

class GameList : public QWidget {
    Q_OBJECT

public:
    GameList(QWidget* parent = 0);
    ~GameList();

public slots:
    void addGame(Game* g);
    void removeGame(Game* g);
    void removeAllGames();
    void orderBy(GameComparator::OrderType type = GameComparator::ByName);

protected:
    virtual void showEvent(QShowEvent* event);

private:
    QList<Game*> m_games;
    bool m_updated;
    GameComparator::OrderType m_order;

private slots:
    void updateGames();

};

#endif // GAMELIST_H
