#include <QDebug>

#include <QBoxLayout>
#include <QResizeEvent>

#include <GameSelector/util/GameInfo.h>
#include <GameSelector/widgets/Game.h>
#include <GameSelector/widgets/GameScroller.h>

#define DefaultBorderSize   4
#define DefaultGameSpacing 25

GameScroller::GameScroller(QWidget* parent) :
    QWidget(parent),
    m_selectionBox(this),
    m_dirty(false) {

    QObject::connect(this, SIGNAL(gamesChanged()), this, SLOT(markDirty()));

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
}

GameScroller::~GameScroller() {
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
        emit gamesChanged();
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
        emit gamesChanged();
}

void GameScroller::removeGame(Game* game) {
    if (m_games.removeAll(game) > 0)
        emit gamesChanged();
}

void GameScroller::removeGames(QList<Game*> games) {
    bool changed = false;

    foreach (Game* game, games) {
        if (m_games.removeAll(game))
           changed = true;
    }

    if (changed)
        emit gamesChanged();
}

void GameScroller::clear() {
    if (m_games.count() > 0) {
        m_games.clear();

        emit gamesChanged();
    }
}

void GameScroller::showEvent(QShowEvent* event) {
    this->rebuild();
    QWidget::showEvent(event);
}

void GameScroller::resizeEvent(QResizeEvent* event) {
    QSize size = event->size();

    int margin = ((size.width() - m_selectionBox.width()) / 2) + DefaultBorderSize;
    m_viewport.layout()->setContentsMargins(margin, 0, (margin + size.width() % 2), 0);

    // int left, int top, int right, int bottom
    m_selectionBox.move(QPoint(((size.width() - m_selectionBox.width()) / 2), 0));
    m_selectionBox.raise();
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

    // Remove all games from the layout.
    QLayoutItem* item;
    while ((item = m_viewport.layout()->takeAt(0)) != 0) {
        if (qobject_cast<Game*>(item->widget()))
            delete item;
    }

    // From this new list, put the games
    // back to the layout.
    foreach (Game* g, m_games)
        m_viewport.layout()->addWidget(g);

    // Mark the state as updated so no
    // unnecessary computations are performed.
    m_dirty = false;
}
