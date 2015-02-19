#ifndef GAMESELECTOR_H
#define GAMESELECTOR_H

#include <QDir>
#include <QList>
#include <QList>
#include <QWidget>

#include <GameSelector/loader/GameLoader.h>

namespace Ui {
    class GameSelector;
}

class Game;

class GameSelector : public QWidget {
    Q_OBJECT

public:
    explicit GameSelector(QDir& root, QWidget *parent = 0);
    virtual ~GameSelector();

public slots:
    bool selectGame(Game* g);
    bool unselectGame(Game* g);

private:
    Ui::GameSelector *ui;
    QDir& m_root;
    GameLoader m_loader;
    Game* m_selected;

private slots:
    void createGame(GameInfo info);
    void showError(const QString& msg);

    void updateGameInfo(Game* game);
    void updateGameIndex(int index);

};

#endif // GAMESELECTOR_H
