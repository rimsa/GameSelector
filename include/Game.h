#ifndef GAME_H
#define GAME_H

#include <QLabel>
#include <GameInfo.h>

class Game : public QLabel {
    Q_OBJECT

public:
    explicit Game(GameInfo info, QWidget *parent = 0);
    virtual ~Game();

    const QString& name() const { return m_info.name; }
    const QString& diskName() const { return m_info.diskName; }
    const QPixmap& cover() const { return m_info.cover; }

    bool hasGenre() const { return (m_info.options & HAS_GENRE); }
    const QString& genre() const { return m_info.genre; }

    bool hasDeveloper() const { return (m_info.options & HAS_DEVELOPER); }
    const QString& developer() const { return m_info.developer; }

    bool hasPublisher() const { return (m_info.options & HAS_PUBLISHER); }
    const QString& publisher() const { return m_info.publisher; }

    bool hasYear() const { return (m_info.options & HAS_YEAR); }
    int year() const { return m_info.year; }

private:
    GameInfo m_info;

};

#endif // GAME_H
