#include <Game.h>

Game::Game(GameInfo info, QWidget *parent) :
    QLabel(parent),
    m_info(info) {

    this->setPixmap(info.cover);
    this->setFixedSize(DefaultCoverSize);
}

Game::~Game() {
}
