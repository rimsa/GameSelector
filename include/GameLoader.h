#ifndef GAMELOADER_H
#define GAMELOADER_H

#include <QObject>

#include <GameInfo.h>

class QSettings;
class QDir;

class GameLoader : public QObject {
    Q_OBJECT

public:
    explicit GameLoader(QDir& root, QObject *parent = 0);
    virtual ~GameLoader();

signals:
    void gameLoaded(GameInfo info);
    void loadingFinished();

public slots:
    void load();

private:
    QDir& m_root;

    bool readField(const QSettings& inf, const QString& dirName, const QString& fieldName, QGenericArgument fieldValue);

private slots:
    void loadGame(QString dirName);

};

#endif // GAMELOADER_H
