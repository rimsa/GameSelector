#ifndef GAMEFILTER_H
#define GAMEFILTER_H

#include <QObject>

#include <GameInfo.h>

class Game;

class GameFilter : public QObject {
    Q_OBJECT

public:
    GameFilter(QObject* parent = 0);
    virtual ~GameFilter();

    bool hasName() const { return m_options & HAS_NAME; }
    const QString& name() const { return m_name; }

    bool hasGenre() const { return m_options & HAS_GENRE; }
    const QString& genre() const { return m_genre; }

    bool hasDeveloper() const { return m_options & HAS_DEVELOPER; }
    const QString& developer() const { return m_developer; }

    bool hasPublisher() const { return m_options & HAS_PUBLISHER; }
    const QString& publisher() const { return m_publisher; }

    bool hasYear() const { return m_options & HAS_YEAR; }
    int year() const { return m_year; }

public slots:
    void setName(const QString& name);
    void unsetName();

    void setGenre(const QString& genre);
    void unsetGenre();

    void setDeveloper(const QString& developer);
    void unsetDeveloper();

    void setPublisher(const QString& publisher);
    void unsetPublisher();

    void setYear(int year);
    void unsetYear();

    void applyFilter(QList<Game*> games);

signals:
    void filterChanged();

private:
    QString m_name;
    QString m_genre;
    QString m_developer;
    QString m_publisher;
    int m_year;

    GameInfoOptions m_options;

};

#endif // GAMEFILTER_H
