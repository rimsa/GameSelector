#include <QDebug>

#include <QIcon>
#include <QStyle>

#include <GameSelector/widgets/Game.h>

Game::Game(GameInfo info, QWidget *parent) :
    QPushButton(parent),
    m_info(info),
    m_selected(false) {

    this->setFlat(true);
    this->setCheckable(false);

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(select()));

    this->setIcon(QIcon(info.cover));
    this->setIconSize(DefaultCoverSize);
    this->setFixedSize(DefaultCoverSize);
}

Game::~Game() {
}

void Game::select() {
    if (!m_selected) {
        m_selected = true;
        this->updateStyleSheet();
        emit selected(this);
    }
}

void Game::unselect() {
    if (m_selected) {
        m_selected = false;
        this->updateStyleSheet();
        emit unselected(this);
    }
}

void Game::changeSelection(bool mode) {
    if (mode)
        this->select();
    else
        this->unselect();
}

void Game::updateStyleSheet() {
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}
