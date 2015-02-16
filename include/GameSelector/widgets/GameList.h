#ifndef GAMELIST_H
#define GAMELIST_H

#include <QWidget>
#include <QScrollArea>

#include <GameSelector/util/GameFilter.h>
#include <GameSelector/util/GameSort.h>

class Game;

class GameList : public QWidget {
    Q_OBJECT

public:
    GameList(QWidget* parent = 0);
    ~GameList();

public slots:
    GameFilter& filter() { return m_filter; }
    GameSort& sort() { return m_sort; }

    void addGame(Game* g);
    void addGames(QList<Game*> games);

    void removeGame(Game* g);
    void removeAllGames();

protected:
    virtual void showEvent(QShowEvent* event);

private:
    QList<Game*> m_games;

    bool m_updated;
    GameFilter m_filter;
    GameSort m_sort;

    QScrollArea m_scrollArea;
    QWidget m_viewport;

private slots:
    void markUpdate();
    void updateGames();

};

#endif // GAMELIST_H
