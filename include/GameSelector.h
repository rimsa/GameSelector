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
    explicit GameSelector(QDir& root, QWidget *parent = 0);
    virtual ~GameSelector();

public slots:
    bool selectGame(Game* g);
    bool unselectGame(Game* g);

private:
    Ui::GameSelector *ui;
    QDir& m_root;
    GameLoader m_loader;
    GameList m_list;
    Game* m_selected;

private slots:
    void createGame(GameInfo info);
    void showError(const QString& msg);

};

#endif // GAMESELECTOR_H
