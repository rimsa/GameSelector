#include <QDebug>

#include <QIcon>

#include <Game.h>

Game::Game(GameInfo info, QWidget *parent) :
    QPushButton(parent),
    m_info(info),
    m_selected(false) {

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
        emit selected(this);
    }
}

void Game::unselect() {
    if (m_selected) {
        m_selected = false;
        emit unselected(this);
    }
}
