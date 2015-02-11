#include <QSettings>
#include "Game.h"

Game::Game(QDir& root, const QString& dirName, QObject *parent) :
    QObject(parent),
    m_root(root),
    m_dirName(dirName),
    m_invalid(true) {
}

Game::~Game() {
}

/*
void Game::load() {
        m_info = info;
        m_invalid = false;
}

bool Game::select() {
    return false;
}

bool Game::unselect() {
    return false;
}

struct GameInfo Game::loadInformation() const {
    QString path = QString("%1/%2/game.inf").arg(m_root.absolutePath()).arg(m_dirName);

    QDir file(path);
    if (!file.exists()) {
        struct GameInfo info;

        QSettings inf(path, QSettings::IniFormat, this);
        inf.beginGroup("Game");

        if (inf.contains("Name"))
            info.name = inf.value("Name").toString();
        else
            return;

        if (inf.contains("DiskName"))
            info.diskName = inf.value("DiskName").toString();
        else
            return;

        if (inf.contains("Cover")) {
            QString imgName = inf.value("Cover").toString();
            info.cover = QPixmap(QString("%1/%2/%3").arg(m_root.absolutePath()).arg(m_dirName).arg(imgName));
        }

        m_info.genre = inf.value("Genre").toString();
        m_info.developer = inf.value("Developer").toString();
        m_info.publisher = inf.value("Publisher").toString();
        m_info.year = inf.value("Year").toInt();

        inf.endGroup();
    }
}
*/
