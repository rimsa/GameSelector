#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QDir>
#include <QSize>
#include <QString>
#include <QPixmap>

enum GameInfoOption {
    HAS_GENRE     = 0x01,
    HAS_DEVELOPER = 0x02,
    HAS_PUBLISHER = 0x04,
    HAS_YEAR      = 0x08
};

Q_DECLARE_FLAGS(GameInfoOptions, GameInfoOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(GameInfoOptions)

struct GameInfo {
    QString dirName;

    QString name;
    QString diskName;
    QPixmap cover;
    QString genre;
    QString developer;
    QString publisher;
    int year;

    GameInfoOptions options;
};

Q_DECLARE_METATYPE(GameInfo)

#define DefaultSelectedGameDir "XBox360"
#define DefaultCoverSize QSize(356,500)

#endif // GAMEINFO_H
