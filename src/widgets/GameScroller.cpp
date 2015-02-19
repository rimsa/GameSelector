#include <QDebug>

#include <QBoxLayout>
#include <QResizeEvent>
#include <QScrollBar>

#include <GameSelector/util/GameInfo.h>
#include <GameSelector/widgets/Game.h>
#include <GameSelector/widgets/GameScroller.h>

#define DefaultBorderSize   4
#define DefaultGameSpacing 25
#define DefaultAnimationTime 500

GameScroller::GameScroller(QWidget* parent) :
    QWidget(parent),
    m_selectionBox(this),
    m_animation(new QPropertyAnimation(this)),
    m_leftSpacer(new QSpacerItem(DefaultGameSpacing, 0, QSizePolicy::Fixed, QSizePolicy::Fixed)),
    m_rightSpacer(new QSpacerItem(DefaultGameSpacing, 0, QSizePolicy::Fixed, QSizePolicy::Fixed)),
    m_timer(new QTimer(this)),
    m_dirty(false),
    m_index(-1) {

    QObject::connect(this, SIGNAL(gamesAvailableChanged()), this, SLOT(markDirty()));

    QSize coverWithBorder(
                DefaultCoverSize.width() + (DefaultBorderSize * 2),
                DefaultCoverSize.height() + (DefaultBorderSize * 2)
                );

    this->setMinimumSize(coverWithBorder);
    this->setMaximumSize(QSize(this->maximumHeight(), coverWithBorder.height()));

    QBoxLayout* viewportLayout = new QHBoxLayout;
    viewportLayout->setSpacing(DefaultGameSpacing);
    viewportLayout->setContentsMargins(0, 0, 0, 0);

    m_viewport.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_viewport.setMinimumHeight(DefaultCoverSize.height());
    m_viewport.setMaximumHeight(DefaultCoverSize.height());
    m_viewport.setLayout(viewportLayout);
    //m_viewport.installEventFilter(this);

    m_scrollArea.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_scrollArea.setMinimumHeight(DefaultCoverSize.height());
    m_scrollArea.setMaximumHeight(DefaultCoverSize.height());
    m_scrollArea.setFrameShape(QFrame::NoFrame);
    m_scrollArea.setFrameShadow(QFrame::Plain);
    m_scrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea.setWidgetResizable(true);
    m_scrollArea.setStyleSheet("background-color: transparent;");
    m_scrollArea.setWidget(&m_viewport);
    //m_scrollArea.installEventFilter(this);

    QBoxLayout* thisLayout = new QVBoxLayout;
    thisLayout->setSpacing(0);
    thisLayout->setContentsMargins(0, DefaultBorderSize, 0, DefaultBorderSize);
    thisLayout->addWidget(&m_scrollArea);

    this->setLayout(thisLayout);

    m_selectionBox.setAttribute(Qt::WA_TransparentForMouseEvents);
    m_selectionBox.setStyleSheet(QString("border: %1px solid white;").arg(DefaultBorderSize));
    m_selectionBox.resize(coverWithBorder);
    m_selectionBox.move(0, 0);

    QScrollBar* scroller = m_scrollArea.horizontalScrollBar();
    QObject::connect(scroller, SIGNAL(valueChanged(int)), this, SLOT(handleScrollerChange(int)));

    m_animation->setTargetObject(scroller);
    m_animation->setPropertyName("value");
    m_animation->setDuration(DefaultAnimationTime);
    m_animation->setEasingCurve(QEasingCurve::OutBack);

    m_timer->setInterval(50);
    m_timer->setSingleShot(true);

    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(animateScroller()));
    QObject::connect(this, SIGNAL(gameChanged(Game*)), this, SLOT(animateScroller()));
}

GameScroller::~GameScroller() {
}

Game* GameScroller::gameAt(int index) const {
    return (index >= 0 && index < this->count() ? m_games.at(index) : 0);
}

int GameScroller::gameIndex(Game* game) const {
    return m_games.indexOf(game);
}

Game* GameScroller::currentGame() const {
    return this->gameAt(m_index);
}

void GameScroller::addGame(Game* game, bool fresh) {
    bool changed = false;

    if (fresh) {
        if (m_games.count() > 0) {
            m_games.clear();

            changed = true;
        }
    }

    if (game && !m_games.contains(game)) {
        m_games.push_back(game);

        changed = true;
    }

    if (changed)
        emit gamesAvailableChanged();
}

void GameScroller::addGames(QList<Game*> games, bool fresh) {
    bool changed = false;

    if (fresh) {
        if (m_games.count() > 0) {
            m_games.clear();

            changed = true;
        }
    }

    foreach (Game* game, games) {
        if (game && !m_games.contains(game)) {
            m_games.push_back(game);

            changed = true;
        }
    }

    if (changed)
        emit gamesAvailableChanged();
}

void GameScroller::removeGame(Game* game) {
    if (m_games.removeAll(game) > 0)
        emit gamesAvailableChanged();
}

void GameScroller::removeGames(QList<Game*> games) {
    bool changed = false;

    foreach (Game* game, games) {
        if (m_games.removeAll(game))
           changed = true;
    }

    if (changed)
        emit gamesAvailableChanged();
}

void GameScroller::clear() {
    if (m_games.count() > 0) {
        m_games.clear();

        emit gamesAvailableChanged();
    }
}

void GameScroller::setCurrentGameIndex(int index, bool force) {
    if (force || (m_index != index && index >= 0 && index < this->count())) {
        // Disable previous current game.
        if (Game* game = this->currentGame())
            game->setDisabled(true);

        // If the index is out of bounds, choose -1.
        m_index = (index >= 0 && index < this->count() ? index : -1);

        // Enable the newly updated current game.
        if (Game* game = this->currentGame())
            game->setEnabled(true);

        emit gameIndexChanged(m_index);
        emit gameChanged(this->gameAt(m_index));
    }
}

void GameScroller::setCurrentGame(Game* game, bool force) {
    this->setCurrentGameIndex(this->gameIndex(game), force);
}

void GameScroller::previousGame() {
    if (m_index == 0) {
        QScrollBar* scroller = m_scrollArea.horizontalScrollBar();
        scroller->setValue(scroller->minimum());

        this->setCurrentGameIndex(m_index, true);
    } else {
        this->setCurrentGameIndex(m_index - 1);
    }
}

void GameScroller::nextGame() {
    if (m_index == (this->count() - 1)) {
        QScrollBar* scroller = m_scrollArea.horizontalScrollBar();
        scroller->setValue(scroller->maximum());

        this->setCurrentGameIndex(m_index, true);
    } else {
        this->setCurrentGameIndex(m_index + 1);
    }
}

void GameScroller::showEvent(QShowEvent* event) {
    this->rebuild();
    QWidget::showEvent(event);
}

void GameScroller::resizeEvent(QResizeEvent* event) {
    // Get the new window size.
    QSize size = event->size();

    // Calculate the correct margin according to the window size.
    int margin = ((size.width() - m_selectionBox.width()) / 2) + DefaultBorderSize;
    m_viewport.layout()->setContentsMargins(margin, 0, (margin + size.width() % 2), 0);

    // Move the selection box to the center.
    m_selectionBox.move(QPoint(((size.width() - m_selectionBox.width()) / 2), 0));
    m_selectionBox.raise();

    // Update game position.
    this->animateScroller();

    QWidget::resizeEvent(event);
}

void GameScroller::wheelEvent(QWheelEvent* event) {
    // Start the timer event.
    m_timer->start();

    QWidget::wheelEvent(event);
}

void GameScroller::markDirty() {
    m_dirty = true;
    this->rebuild();
}

void GameScroller::rebuild() {
    // If already updated, no need to recompute.
    if (!m_dirty)
        return;

    // Only compute list if visible,
    // otherwise postpone it until
    // the widget is visible.
    if (!this->isVisible())
        return;

    // Save the current game.
    Game* currentGame = this->currentGame();

    // Remove all items from the layout.
    QLayoutItem* item;
    while ((item = m_viewport.layout()->takeAt(0)) != 0) {
        // If it is a game, release the memory.
        if (qobject_cast<Game*>(item->widget()))
            delete item;
    }

    // Get the viewport layout.
    QLayout* layout = m_viewport.layout();

    // Add left spacer.
    layout->addItem(m_leftSpacer);

    // From this new list, put the games
    // back to the layout.
    foreach (Game* game, m_games) {
        // Disable the game before adding it.
        game->setDisabled(true);

        // Add the game to the layout.
        layout->addWidget(game);
    }

    // Add left spacer.
    layout->addItem(m_rightSpacer);

    // If the current game exists in the
    // the list, selected it again.
    if (m_games.contains(currentGame))
        this->setCurrentGame(currentGame);
    // If not and there is at least
    // one game, select it.
    else if (this->count() > 0)
        this->setCurrentGameIndex(0);
    // Otherwise, select none.
    else
        this->setCurrentGameIndex(-1, true);

    // Mark the state as updated so no
    // unnecessary computations are performed.
    m_dirty = false;
}

void GameScroller::handleScrollerChange(int value) {
    // As long as the timer is active,
    // calculate the scroller last
    // game position.
    if (m_timer->isActive()) {
        // Stop any animation if it is running.
        m_animation->stop();

        // Calculate the new game index and update it.
        int index = (value - DefaultGameSpacing + (DefaultCoverSize.width() / 2)) /
                    (DefaultCoverSize.width() + DefaultGameSpacing);
        this->setCurrentGameIndex(index);

        // Restart the timer.
        m_timer->start();
    }
}

void GameScroller::animateScroller() {
    // Do not animate if the timer is active.
    if (m_timer->isActive())
        return;
    // If the animation is already running, don't start over.
    else if (m_animation->state() == QAbstractAnimation::Running)
        return;

    // Get the current index.
    int index = this->currentGameIndex();

    // Get the horizontal scrollbar.
    QScrollBar* scroller = m_scrollArea.horizontalScrollBar();
    if (index < 0) {
        scroller->setValue(0);
    } else {
        int from = scroller->value();
        int to = DefaultGameSpacing + (index * (DefaultCoverSize.width() + DefaultGameSpacing));

        m_animation->setStartValue(from);
        m_animation->setEndValue(to);
        m_animation->start();
    }
}
