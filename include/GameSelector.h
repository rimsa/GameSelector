#ifndef GAMESELECTOR_H
#define GAMESELECTOR_H

#include <QDir>
#include <QList>
#include <QList>
#include <QWidget>

#include <GameList.h>
#include <GameLoader.h>

namespace Ui {
    class GameSelector;
}

class Game;

class GameSelector : public QWidget {
    Q_OBJECT

public:
    explicit GameSelector(const QString& root, QWidget *parent = 0);
    virtual ~GameSelector();

private:
    Ui::GameSelector *ui;
    QDir m_root;
    GameLoader m_loader;
    GameList m_list;

private slots:
    void createGame(GameInfo info);

};

#endif // GAMESELECTOR_H
