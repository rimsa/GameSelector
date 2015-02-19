#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QDir>
#include <QSize>
#include <QColor>
#include <QString>
#include <QPixmap>

enum GameInfoOption {
    HAS_NAME      = 0x0001,
    HAS_DISKNAME  = 0x0002,
    HAS_COVER     = 0x0004,
    HAS_ARCADE    = 0x0008,
    HAS_GENRE     = 0x0010,
    HAS_DEVELOPER = 0x0020,
    HAS_PUBLISHER = 0x0040,
    HAS_YEAR      = 0x0080,
    HAS_SIZE      = 0x0100
};

Q_DECLARE_FLAGS(GameInfoOptions, GameInfoOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(GameInfoOptions)

struct GameInfo {
    QString dirName;

    QString name;
    QString diskName;
    QPixmap cover;
    bool arcade;
    QString genre;
    QString developer;
    QString publisher;
    int year;
    float size;

    GameInfoOptions options;
};

Q_DECLARE_METATYPE(GameInfo)

#define DefaultSelectedGameDir "Xbox360"
#define DefaultCoverSize QSize(214,300)
#define DefaultBorderSize   4
#define DefaultGameSpacing 25
#define DefaultAnimationTime 500
#define DefaultSelectionColor QColor(Qt::green)

#endif // GAMEINFO_H
