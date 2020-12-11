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
    m_selected(0),
    m_sort(GameSort::ByName, Qt::AscendingOrder, this) {

    ui->setupUi(this);

    QObject::connect(&m_loader, SIGNAL(gameLoaded(GameInfo)), this, SLOT(createGame(GameInfo)));
    QObject::connect(&m_loader, SIGNAL(loadingFinished()), this, SLOT(loadGames()), Qt::QueuedConnection);
    QObject::connect(&m_loader, SIGNAL(loadingFinished()), this, SLOT(show()), Qt::QueuedConnection);

    QObject::connect(ui->games, SIGNAL(gameChanged(Game*)), this, SLOT(updateGameInfo(Game*)));
    QObject::connect(ui->games, SIGNAL(gameIndexChanged(int)), this, SLOT(updateGameIndex(int)));

    QObject::connect(ui->orderTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOrder()));
    QObject::connect(ui->orderSortCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOrder()));

    QMetaObject::invokeMethod(&m_loader, "load", Qt::QueuedConnection);
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
    if (m_selected == ui->games->currentGame())
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

void GameSelector::showEvent(QShowEvent* event) {
    QMetaObject::invokeMethod(this, "showFullScreen", Qt::QueuedConnection);
    return QWidget::showEvent(event);
}

void GameSelector::createGame(GameInfo info) {
    // Create a new game.
    Game* g = new Game(info);

    // A game must not be initially selected.
    Q_ASSERT(!g->isSelected());

    // If the game is selected, take the appropriate actions here.
    QObject::connect(g, SIGNAL(selected(Game*)), this, SLOT(selectGame(Game*)), Qt::DirectConnection);
    QObject::connect(g, SIGNAL(unselected(Game*)), this, SLOT(unselectGame(Game*)), Qt::DirectConnection);
    QObject::connect(g, SIGNAL(selectionChanged(bool)), ui->games, SLOT(updateGameSelection()));

    // Select the game if matched the default directory.
    if (g->dirName() == DefaultSelectedGameDir)
        g->select();

    // Added to the list.
    m_games.push_back(g);
}

void GameSelector::loadGames() {
    // Create a new list that can be manipulated.
    QList<Game*> displayGames(m_games);

    // Putting games in order.
    m_sort.sort(displayGames);

    // Add these games to the scroller.
    ui->games->addGames(displayGames, true);
}

void GameSelector::updateOrder() {
    switch (ui->orderTypeCombo->currentIndex()) {
        case 0:
            m_sort.setType(GameSort::ByName);
            break;
        case 1:
            m_sort.setType(GameSort::ByGenre);
            break;
        case 2:
            m_sort.setType(GameSort::ByDeveloper);
            break;
        case 3:
            m_sort.setType(GameSort::ByPublisher);
            break;
        case 4:
            m_sort.setType(GameSort::ByYear);
            break;
        case 5:
            m_sort.setType(GameSort::BySize);
            break;
        default:
            Q_ASSERT("Invalid sort type");
            break;
    }

    switch (ui->orderSortCombo->currentIndex()) {
        case 0:
            m_sort.setOrder(Qt::AscendingOrder);
            break;
        case 1:
            m_sort.setOrder(Qt::DescendingOrder);
            break;
        default:
            Q_ASSERT("Invalid sort mode");
            break;
    }

    QMetaObject::invokeMethod(this, "loadGames", Qt::QueuedConnection);
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
        ui->mediaValue->setText(game->hasMedia() ? game->media() : "");
        ui->arcadeValue->setText(game->arcade() ? trUtf8("Yes") : trUtf8("No"));
        ui->genreValue->setText(game->hasGenre() ? game->genre() : "");
        ui->developerValue->setText(game->hasDeveloper() ? game->developer() : "");
        ui->publisherValue->setText(game->hasPublisher() ? game->publisher() : "");
        ui->yearValue->setText(game->hasYear() ? QString::number(game->year()) : "");
        ui->sizeValue->setText(game->hasSize() ? QString("%1 GB").arg(game->size()) : "");
    } else {
        ui->nameValue->setText("");
        ui->mediaValue->setText("");
        ui->arcadeValue->setText("");
        ui->genreValue->setText("");
        ui->developerValue->setText("");
        ui->publisherValue->setText("");
        ui->yearValue->setText("");
        ui->sizeValue->setText("");
    }

    ui->leftButton->setVisible(ui->games->hasPrevious());
    ui->rightButton->setVisible(ui->games->hasNext());
}

void GameSelector::updateGameIndex(int index) {
    ui->gameIndex->setText(index < 0 ? "" : QString("%1 / %2").arg(index+1).arg(ui->games->count()));
}
