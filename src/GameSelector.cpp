#include <QDebug>

#include <QFile>
#include <QSettings>
#include <QMetaObject>
#include <QMessageBox>

#include <GameSelector/GameSelector.h>
#include <GameSelector/widgets/Game.h>

#include "ui_GameSelector.h"

GameSelector::GameSelector(QDir& root, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameSelector),
    m_root(root),
    m_loader(root),
    m_selected(0) {

    ui->setupUi(this);

    QObject::connect(&m_loader, SIGNAL(gameLoaded(GameInfo)), this, SLOT(createGame(GameInfo)));
    QObject::connect(&m_loader, SIGNAL(loadingFinished()), this, SLOT(show()), Qt::QueuedConnection);

    QMetaObject::invokeMethod(&m_loader, "load", Qt::QueuedConnection);

    this->adjustSize();
    this->setFixedSize(this->size());
}

GameSelector::~GameSelector() {
    delete ui;
}

bool GameSelector::selectGame(Game* g) {
    // Selecting the same game has no effect.
    if (m_selected == g)
        return true;

    // The game must be selected to mantain state.
    if (!g->isSelected())
        return false;

    // If there's an already selected game.
    if (m_selected)
        m_selected->unselect();

    // After the game is unselect,
    // the state must follow
    if (m_selected)
        return false;

    if (g->dirName() != DefaultSelectedGameDir) {
        QString oldName(g->dirName());
        QString newName(DefaultSelectedGameDir);

        if (!m_root.rename(oldName, newName)) {
            qDebug() << "Unable to rename" << oldName << " to" << newName;

            this->showError(tr("Unable to select game: %1").arg(g->name()));
            return false;
        }

        g->setDirName(newName);
    }

    m_selected = g;

    // Update game information.
    this->updateGameInfo(m_selected);

    return true;
}

bool GameSelector::unselectGame(Game* g) {
    // Unselecting a game that is not selected has no effect
    if (m_selected != g)
        return true;

    // The game must be unselected to mantain state.
    if (g->isSelected())
        return false;

    // Attempting to rename
    QString oldName(DefaultSelectedGameDir);
    QString newName(QString("%1-%2").arg(DefaultSelectedGameDir).arg(g->diskName()));
    if (!m_root.rename(oldName, newName)) {
        qDebug() << "Unable to rename" << oldName << "to" << newName;

        this->showError(tr("Unable to unselect game: %1").arg(g->name()));
        return false;
    }

    g->setDirName(newName);

    m_selected = 0;

    return true;
}

void GameSelector::createGame(GameInfo info) {
    // Create a new game.
    Game* g = new Game(info);

    // A game must not be initially selected.
    Q_ASSERT(!g->isSelected());

    // If the game is selected, take the appropriate actions here.
    QObject::connect(g, SIGNAL(selected(Game*)), this, SLOT(selectGame(Game*)), Qt::DirectConnection);
    QObject::connect(g, SIGNAL(unselected(Game*)), this, SLOT(unselectGame(Game*)), Qt::DirectConnection);

    if (g->dirName() == DefaultSelectedGameDir)
        g->select();

    ui->gameList->addGame(g);
}

void GameSelector::showError(const QString& msg) {
    QMessageBox::critical(this, tr("Game Selector"),
                                tr("%1\n\nThe application will be closed!").arg(msg),
                                QMessageBox::Ok);
    qApp->exit();
}

void GameSelector::updateGameInfo(Game* game) {
    if (game) {
        ui->nameValue->setText(game->name());
        ui->genreValue->setText(game->hasGenre() ? game->genre() : "");
        ui->developerValue->setText(game->hasDeveloper() ? game->developer() : "");
        ui->publisherValue->setText(game->hasPublisher() ? game->publisher() : "");
        ui->yearValue->setText(game->hasYear() ? QString::number(game->year()) : "");
        ui->selectedValue->setText(game->isSelected() ? tr("Yes") : tr("No"));
    } else {
        ui->nameValue->setText("");
        ui->genreValue->setText("");
        ui->developerValue->setText("");
        ui->publisherValue->setText("");
        ui->yearValue->setText("");
    }
}

void GameSelector::updateGameIndex(int index) {
    ui->gameIndex->setText(index < 0 ? "" : QString("%1/%2").arg(index+1).arg(ui->gameList->displayCount()));
}
