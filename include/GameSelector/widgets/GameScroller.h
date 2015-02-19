#ifndef GAMESCROLLER_H
#define GAMESCROLLER_H

#include <QWidget>
#include <QScrollArea>

class Game;

class GameScroller : public QWidget {
    Q_OBJECT

public:
    GameScroller(QWidget* parent = 0);
    virtual ~GameScroller();

public slots:
    void addGame(Game* game, bool fresh = false);
    void addGames(QList<Game*> games, bool fresh = false);
    void removeGame(Game* game);
    void removeGames(QList<Game*> games);
    void clear();

signals:
    void gamesChanged();

protected:
    virtual void showEvent(QShowEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

private:
    QScrollArea m_scrollArea;
    QWidget m_viewport;
    QWidget m_selectionBox;

    QList<Game*> m_games;
    bool m_dirty;

private slots:
    void markDirty();
    void rebuild();

};

#endif // GAMESCROLLER_H
