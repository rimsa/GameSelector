#ifndef GAME_H
#define GAME_H

#include <QDir>
#include <QLabel>
#include <QPushButton>

#include <GameInfo.h>

class Game : public QPushButton {
    Q_OBJECT

public:
    Game(GameInfo info, QWidget *parent = 0);
    virtual ~Game();

    const QString& dirName() const { return m_info.dirName; }
    void setDirName(const QString& dirName) { Q_ASSERT(!dirName.isEmpty()); m_info.dirName = dirName; }

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

    bool isSelected() const { return m_selected; }

public slots:
    void select();
    void unselect();

signals:
    void selected(Game*);
    void unselected(Game*);

private:
    GameInfo m_info;
    bool m_selected;

};

#endif // GAME_H
