#ifndef GAMELIST_H
#define GAMELIST_H

#include <QEvent>
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

    bool eventFilter(QObject* watched, QEvent* event);

    QList<Game*> allGames() const { return m_allGames; }
    QList<Game*> displayGames() const { return m_displayGames; }

    int count() const { return m_allGames.count(); }
    int displayCount() const { return m_displayGames.count(); }

    Game* game() const { return gameByIndex(m_index); }
    int gameIndex() const { return m_index; }

    Game* gameByIndex(int index) const;
    int indexByGame(Game* game) const;

    bool hasPrevious() const { return ((m_index - 1) >= 0 && this->displayCount() > 0); }
    bool hasNext() const { return ((m_index + 1) < this->displayCount()); }

    GameFilter& filter() { return m_filter; }
    GameSort& sort() { return m_sort; }

public slots:
    void addGame(Game* g);
    void addGames(QList<Game*> games);

    void removeGame(Game* g);
    void removeGames();

    void setGame(Game* g, bool force = false);
    void setGameByIndex(int index, bool force = false);

    void previousGame();
    void nextGame();

signals:
    void gameIndexChanged(int index);
    void gameChanged(Game* game);

protected:
    virtual void showEvent(QShowEvent* event);

private:
    QList<Game*> m_allGames;
    QList<Game*> m_displayGames;
    int m_index;

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
