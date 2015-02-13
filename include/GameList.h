#ifndef GAMELIST_H
#define GAMELIST_H

#include <QWidget>

class Game;

class GameList : public QWidget {
    Q_OBJECT

public:
    GameList(QWidget* parent = 0);
    ~GameList();

public slots:
    void addGame(Game* g);

private:
    QList<Game*> m_games;

};

#endif // GAMELIST_H
