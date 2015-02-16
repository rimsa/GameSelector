#include <QMutableListIterator>

#include <Game.h>
#include <GameFilter.h>

GameFilter::GameFilter(QObject* parent) :
    QObject(parent),
    m_options(0) {
}

GameFilter::~GameFilter() {
}

void GameFilter::setName(const QString& name) {
    if (!name.isEmpty() && m_name != name) {
        m_name = name;
        m_options |= HAS_NAME;

        emit filterChanged();
    }
}

void GameFilter::unsetName() {
    if (m_options & HAS_NAME) {
        m_options &= ~HAS_NAME;

        emit filterChanged();
    }
}

void GameFilter::setGenre(const QString& genre) {
    if (!genre.isEmpty() && m_genre != genre) {
        m_genre = genre;
        m_options |= HAS_GENRE;

        emit filterChanged();
    }
}

void GameFilter::unsetGenre() {
    if (m_options & HAS_GENRE) {
        m_options &= ~HAS_GENRE;

        emit filterChanged();
    }
}

void GameFilter::setDeveloper(const QString& developer) {
    if (!developer.isEmpty() && m_developer != developer) {
        m_developer = developer;
        m_options |= HAS_DEVELOPER;

        emit filterChanged();
    }
}

void GameFilter::unsetDeveloper() {
    if (m_options & HAS_DEVELOPER) {
        m_options &= ~HAS_DEVELOPER;

        emit filterChanged();
    }
}

void GameFilter::setPublisher(const QString& publisher) {
    if (!publisher.isEmpty() && m_publisher != publisher) {
        m_publisher = publisher;
        m_options |= HAS_PUBLISHER;

        emit filterChanged();
    }
}

void GameFilter::unsetPublisher() {
    if (m_options & HAS_PUBLISHER) {
        m_options &= ~HAS_PUBLISHER;

        emit filterChanged();
    }
}

void GameFilter::setYear(int year) {
    if (year > 0 && m_year != year) {
        m_year = year;
        m_options |= HAS_YEAR;

        emit filterChanged();
    }
}

void GameFilter::unsetYear() {
    if (m_options & HAS_YEAR) {
        m_options &= ~HAS_YEAR;

        emit filterChanged();
    }
}

void GameFilter::applyFilter(QList<Game*> games) {
    // Filtering by name if present.
    if (this->hasName()) {
        QMutableListIterator<Game*> it(games);
        while (it.hasNext()) {
            Game* g = it.next();
            if (!g->name().contains(this->name()))
                it.remove();
        }
    }

    // Filtering by genre if present.
    if (this->hasGenre()) {
        QMutableListIterator<Game*> it(games);
        while (it.hasNext()) {
            Game* g = it.next();
            if (g->genre() != this->genre())
                it.remove();
        }
    }

    // Filtering by developer if present.
    if (this->hasDeveloper()) {
        QMutableListIterator<Game*> it(games);
        while (it.hasNext()) {
            Game* g = it.next();
            if (g->developer() != this->developer())
                it.remove();
        }
    }

    // Filtering by publisher if present.
    if (this->hasPublisher()) {
        QMutableListIterator<Game*> it(games);
        while (it.hasNext()) {
            Game* g = it.next();
            if (g->publisher() != this->publisher())
                it.remove();
        }
    }

    // Filtering by year if present.
    if (this->hasYear()) {
        QMutableListIterator<Game*> it(games);
        while (it.hasNext()) {
            Game* g = it.next();
            if (g->year() != this->year())
                it.remove();
        }
    }
}
