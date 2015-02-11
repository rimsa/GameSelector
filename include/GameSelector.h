#ifndef GAMESELECTOR_H
#define GAMESELECTOR_H

#include <QDir>
#include <QList>
#include <QWidget>

#include <GameLoader.h>

namespace Ui {
    class GameSelector;
}

class Game;

class GameSelector : public QWidget {
    Q_OBJECT

public:
    explicit GameSelector(QWidget *parent = 0);
    virtual ~GameSelector();

private slots:
    void createGame(GameInfo info);

private:
    Ui::GameSelector *ui;
    QDir m_root;
    GameLoader m_loader;

};

#endif // GAMESELECTOR_H
