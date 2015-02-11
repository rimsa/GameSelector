#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QPixmap>
#include <QDir>
#include "Genre.h"
#include "GameInfo.h"

#define SELECTED_DIRNAME "Xbox360"

class Game : public QObject {
    Q_OBJECT

public:
    explicit Game(QDir& root, const QString& dirName, QObject* parent = 0);
    ~Game();

    bool isSelected() const {
        return (m_dirName == SELECTED_DIRNAME);
    }

    const QString& dirName() const { return m_dirName; }
    bool invalid() const { return m_invalid; }

    const struct GameInfo& info() const { return m_info; }
    const QString& name() const { return m_info.name; }
    const QString& diskName() const { return m_info.diskName; }
    const QPixmap& cover() const { return m_info.cover; }
    const QString& genre() const { return m_info.genre; }
    const QString& developer() const { return m_info.developer; }
    const QString& publisher() const { return m_info.publisher; }
    int year() const { return m_info.year; }

signals:

public slots:
//    void load();
//    bool select();
//    bool unselect();

private:
    QDir m_root;
    QString m_dirName;
    bool m_invalid;
    struct GameInfo m_info;

};

#endif // GAME_H
