#include <QDebug>

#include <QBoxLayout>
#include <QKeyEvent>
#include <QLayout>
#include <QScrollArea>
#include <QScrollBar>

#include <GameSelector/widgets/Game.h>
#include <GameSelector/widgets/GameList.h>

#define DefaultGameSpacing 0

GameList::GameList(QWidget* parent) :
    QWidget(parent),
    m_index(-1),
    m_updated(true),
    m_filter(this),
    m_sort(GameSort::ByName, this),
    m_scrollArea(this),
    m_viewport() {

    QBoxLayout* viewportLayout = new QHBoxLayout;
    viewportLayout->setSpacing(DefaultGameSpacing);
    viewportLayout->setContentsMargins(0, 0, 0, 0);
    m_viewport.setMinimumSize(QSize(0, DefaultCoverSize.height()));
    m_viewport.setMaximumSize(QSize(16777215, DefaultCoverSize.height()));
    //m_viewport.setFixedSize(DefaultCoverSize);
    m_viewport.setLayout(viewportLayout);
    m_viewport.installEventFilter(this);

    m_scrollArea.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_scrollArea.setMinimumSize(DefaultCoverSize);
    m_scrollArea.setMaximumSize(DefaultCoverSize);
    m_scrollArea.setFrameShape(QFrame::NoFrame);
    m_scrollArea.setFrameShadow(QFrame::Plain);
    m_scrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea.setWidgetResizable(true);
    m_scrollArea.setWidget(&m_viewport);
    m_scrollArea.installEventFilter(this);

    QScrollBar* scroll = m_scrollArea.horizontalScrollBar();
    scroll->setSingleStep(DefaultCoverSize.width());
    scroll->setPageStep(DefaultCoverSize.width());

    QObject::connect(&m_filter, SIGNAL(filterChanged()), this, SLOT(markUpdate()));
    QObject::connect(&m_sort, SIGNAL(typeChanged(GameSort::OrderType)), this, SLOT(markUpdate()));

    this->setFixedSize(DefaultCoverSize);
}

GameList::~GameList() {
}

bool GameList::eventFilter(QObject* watched, QEvent* event) {
    Q_UNUSED(watched);

    switch (event->type()) {
#ifndef QT_NO_GESTURES
        // Ignore native gestures.
        case QEvent::NativeGesture:
            return true;
#endif
        // Ignore all mouse wheel events.
        case QEvent::Wheel:
            return true;
        // Ignore all key presses.
        case QEvent::KeyPress:
            return true;
        // Ignore all key presses.
        case QEvent::KeyRelease:
            switch (static_cast<QKeyEvent*>(event)->key()) {
                case Qt::Key_Left:
                    QMetaObject::invokeMethod(this, "previousGame", Qt::QueuedConnection);
                    break;
                case Qt::Key_Right:
                    QMetaObject::invokeMethod(this, "nextGame", Qt::QueuedConnection);
                    break;
                default:
                    break;
            }

            return true;
        default:
            break;
    }

    return false;
}

Game* GameList::gameByIndex(int index) const {
    return (index >= 0 && this->displayCount() ?
                m_displayGames[index] : 0);
}

int GameList::indexByGame(Game* game) const {
    return m_displayGames.indexOf(game);
}

void GameList::addGame(Game* g) {
    m_allGames.push_back(g);

    this->markUpdate();
}

void GameList::addGames(QList<Game*> games) {
    m_allGames.append(games);

    this->markUpdate();
}

void GameList::removeGame(Game* g) {
    m_allGames.removeAll(g);

    this->markUpdate();
}

void GameList::removeGames() {
    m_allGames.clear();

    this->markUpdate();
}

void GameList::setGame(Game* g, bool force) {
    int index = m_displayGames.indexOf(g);
    if (index != m_index || force) {
        QScrollBar* scroller = m_scrollArea.horizontalScrollBar();
        scroller->setValue(index < 0 ? 0 : (index * DefaultCoverSize.width()));

        m_index = index;

        emit gameChanged(this->game());
        emit gameIndexChanged(m_index);
    }
}

void GameList::setGameByIndex(int index, bool force) {
    if (index >= 0 && index < this->displayCount())
        this->setGame(m_displayGames[index], force);
}

void GameList::previousGame() {
    return this->setGameByIndex(m_index - 1);
}

void GameList::nextGame() {
    return this->setGameByIndex(m_index + 1);
}

void GameList::showEvent(QShowEvent* event) {
    QMetaObject::invokeMethod(this, "updateGames", Qt::QueuedConnection);
    QWidget::showEvent(event);
}

void GameList::markUpdate() {
    m_updated = false;
    QMetaObject::invokeMethod(this, "updateGames", Qt::QueuedConnection);
}

void GameList::updateGames() {
    // If already updated, no need to recompute.
    if (m_updated)
        return;

    // Only compute list if visible,
    // otherwise postpone it until
    // the widget is visible.
    if (!this->isVisible())
        return;

    // Remove all games from the layout.
    QLayoutItem* item;
    while ((item = m_viewport.layout()->takeAt(0)) != 0) {
        if (qobject_cast<Game*>(item->widget()))
            delete item;
    }

    // The current game.
    Game* game = this->game();

    // Build a new list with all the games.
    m_displayGames.clear();
    m_displayGames.append(m_allGames);

    // Filter the games by the selected criterias.
    m_filter.applyFilter(m_displayGames);

    // Sort the games according to witch criteria.
    m_sort.sort(m_displayGames);

    // From this new list, put the games
    // back to the layout.
    foreach (Game* g, m_displayGames)
        m_viewport.layout()->addWidget(g);

    // Select the same game if it is
    // still present in the display list.
    if (m_displayGames.contains(game))
        this->setGame(game, true);
    else
        this->setGameByIndex(0, true);

    // Mark the state as updated so no
    // unnecessary computations are performed.
    m_updated = true;
}
