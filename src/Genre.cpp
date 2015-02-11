#include <QDebug>

#include "Genre.h"

QMap<QString, Genre*> Genre::m_genres = QMap<QString, Genre*>();

const Genre& Genre::Arcade = Genre::getGenre("Arcade");
const Genre& Genre::Adventure = Genre::getGenre("Adventure");
const Genre& Genre::RPG = Genre::getGenre("RPG");
const Genre& Genre::Fight = Genre::getGenre("Fight");
const Genre& Genre::Survival = Genre::getGenre("Survival");
const Genre& Genre::Shooter = Genre::getGenre("Shooter");
const Genre& Genre::Platform = Genre::getGenre("Platform");
const Genre& Genre::Racing = Genre::getGenre("Racing");
const Genre& Genre::Strategy = Genre::getGenre("Strategy");

Genre::Genre(const QString& name, QObject *parent) :
    QObject(parent),
    m_name(name) {
}

Genre::~Genre() {
}

const Genre& Genre::getGenre(const QString& genre) {
    Genre* g = m_genres[genre];
    if (!g)
        g = (m_genres[genre] = new Genre(genre));

    return *g;
}
