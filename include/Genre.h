#ifndef GENRE_H
#define GENRE_H

#include <QMap>
#include <QObject>
#include <QString>

class Genre : public QObject {
    Q_OBJECT

public:
    static const Genre& Arcade;
    static const Genre& Adventure;
    static const Genre& RPG;
    static const Genre& Fight;
    static const Genre& Survival;
    static const Genre& Shooter;
    static const Genre& Platform;
    static const Genre& Racing;
    static const Genre& Strategy;

    virtual ~Genre();

    static const Genre& getGenre(const QString& genre);

    const QString& name() const { return m_name; }

private:
    QString m_name;
    static QMap<QString, Genre*> m_genres;

    Genre(const QString& name, QObject* parent = 0);

};

#endif // CATEGORY_H
