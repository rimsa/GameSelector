#ifndef GAME_H
#define GAME_H

#include <QDir>
#include <QEvent>
#include <QLabel>
#include <QPushButton>

#include <GameSelector/util/GameInfo.h>

class Game : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(bool selected READ isSelected WRITE changeSelection NOTIFY selectedChanged)

public:
    Game(GameInfo info, QWidget *parent = 0);
    virtual ~Game();

    virtual bool event(QEvent* e);

    const QString& dirName() const { return m_info.dirName; }
    void setDirName(const QString& dirName) { Q_ASSERT(!dirName.isEmpty()); m_info.dirName = dirName; }

    const QString& name() const { return m_info.name; }
    const QString& diskName() const { return m_info.diskName; }
    const QPixmap& cover() const { return m_info.cover; }
    bool arcade() const { return m_info.arcade; }

    bool hasGenre() const { return (m_info.options & HAS_GENRE); }
    const QString& genre() const { return m_info.genre; }

    bool hasDeveloper() const { return (m_info.options & HAS_DEVELOPER); }
    const QString& developer() const { return m_info.developer; }

    bool hasPublisher() const { return (m_info.options & HAS_PUBLISHER); }
    const QString& publisher() const { return m_info.publisher; }

    bool hasYear() const { return (m_info.options & HAS_YEAR); }
    int year() const { return m_info.year; }

    bool hasSize() const { return (m_info.options & HAS_SIZE); }
    float size() const { return m_info.size; }

    bool isSelected() const { return m_selected; }

public slots:
    void select();
    void unselect();
    void changeSelection(bool mode);

signals:
    void selected(Game*);
    void unselected(Game*);
    void selectedChanged(bool mode);

private:
    GameInfo m_info;
    bool m_selected;

    void updateStyleSheet();
};

#endif // GAME_H
