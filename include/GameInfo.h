#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QSize>
#include <QString>
#include <QPixmap>

enum GameInfoOption {
    HAS_COVER     = 0x01,
    HAS_GENRE     = 0x02,
    HAS_DEVELOPER = 0x04,
    HAS_PUBLISHER = 0x08,
    HAS_YEAR      = 0x10
};

Q_DECLARE_FLAGS(GameInfoOptions, GameInfoOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(GameInfoOptions)

struct GameInfo {
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

#define DefaultCoverSize QSize(356,500)


#endif // GAMEINFO_H
