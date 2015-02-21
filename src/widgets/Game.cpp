#include <QDebug>

#include <QIcon>
#include <QResizeEvent>

#include <GameSelector/widgets/Game.h>

Game::Game(GameInfo info, QWidget *parent) :
    QPushButton(parent),
    m_info(info),
    m_selectionMask(this),
    m_selected(false) {

    this->setFlat(true);
    this->setCheckable(false);
    this->setIcon(QIcon(info.cover));
    this->setIconSize(DefaultCoverSize);
    this->setFixedSize(DefaultCoverSize);

    m_selectionMask.setGeometry(0, 0, this->width(), this->height());
    m_selectionMask.setStyleSheet(
        QString("background-color: rgba(%1, %2, %3, 75);")
                .arg(DefaultSelectionColor.red())
                .arg(DefaultSelectionColor.green())
                .arg(DefaultSelectionColor.blue()));
    m_selectionMask.hide();

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(invertSelection()));
}

Game::~Game() {
}

bool Game::event(QEvent* event) {
    switch (event->type()) {
        case QEvent::EnabledChange:
            this->setAttribute(Qt::WA_TransparentForMouseEvents, !this->isEnabled());
            break;
        default:
            break;
    }

    return QPushButton::event(event);
}

void Game::select() {
    this->changeSelection(true);
}

void Game::unselect() {
    this->changeSelection(false);
}

void Game::changeSelection(bool mode) {
    if (m_selected != mode) {
        m_selected = mode;

        if (mode) {
            m_selectionMask.show();
            m_selectionMask.raise();

            emit selected(this);
        } else {
            m_selectionMask.hide();

            emit unselected(this);
        }

        emit selectionChanged(m_selected);
    }
}

void Game::invertSelection() {
    this->changeSelection(!m_selected);
}

void Game::showEvent(QShowEvent* event) {
    QPushButton::showEvent(event);
}

void Game::resizeEvent(QResizeEvent* event) {
    m_selectionMask.resize(event->size());
    QPushButton::resizeEvent(event);
}
