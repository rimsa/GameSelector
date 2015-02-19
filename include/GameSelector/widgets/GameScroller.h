#ifndef GAMESCROLLER_H
#define GAMESCROLLER_H

#include <QTimer>
#include <QWidget>
#include <QScrollArea>
#include <QSpacerItem>
#include <QPropertyAnimation>

class Game;

class GameScroller : public QWidget {
    Q_OBJECT

public:
    GameScroller(QWidget* parent = 0);
    virtual ~GameScroller();

    int count() const { return m_games.count(); }
    Game* gameAt(int index) const;
    int gameIndex(Game* game) const;

    int currentGameIndex() const { return m_index; }
    Game* currentGame() const;

public slots:
    void addGame(Game* game, bool fresh = false);
    void addGames(QList<Game*> games, bool fresh = false);
    void removeGame(Game* game);
    void removeGames(QList<Game*> games);
    void clear();

    void setCurrentGameIndex(int index, bool force = false);
    void setCurrentGame(Game* game, bool force = false);

    void previousGame();
    void nextGame();

    void updateGameSelection();

signals:
    void gamesAvailableChanged();

    void gameChanged(Game* game);
    void gameIndexChanged(int index);

protected:
    virtual void showEvent(QShowEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void wheelEvent(QWheelEvent* event);

private:
    QScrollArea m_scrollArea;
    QWidget m_viewport;
    QWidget m_selectionBox;
    QPropertyAnimation* m_animation;
    QSpacerItem* m_leftSpacer;
    QSpacerItem* m_rightSpacer;
    QTimer* m_timer;
    QList<Game*> m_games;
    bool m_dirty;

    int m_index;

private slots:
    void markDirty();
    void rebuild();

    void handleScrollerChange(int value);
    void animateScroller();

};

#endif // GAMESCROLLER_H
