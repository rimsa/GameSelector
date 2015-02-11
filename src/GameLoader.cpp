#include <QDebug>

#include <QDir>
#include <QSettings>
#include <QGenericArgument>
#include <cstdlib>

#include <GameInfo.h>
#include <GameLoader.h>

GameLoader::GameLoader(QDir& root, QObject *parent) :
    QObject(parent), m_root(root) {
}

GameLoader::~GameLoader() {
}

void GameLoader::load() {
    m_root.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    QFileInfoList list = m_root.entryInfoList();
    foreach (QFileInfo fileInfo, list)
        QMetaObject::invokeMethod(this, "loadGame", Qt::QueuedConnection, Q_ARG(QString, fileInfo.fileName()));

    emit loadingFinished();
}

void GameLoader::loadGame(QString dirName) {
    QDir file(QString("%1/%2/game.inf").arg(m_root.absolutePath()).arg(dirName));
    if (!file.exists()) {
        struct GameInfo info;
        info.options = 0;

        QSettings inf(file.absolutePath(), QSettings::IniFormat, this);
        inf.beginGroup("Game");

        struct Field {
            QString fieldName;
            QGenericArgument fieldValue;
            bool required;
            GameInfoOptions option;
        } fields[] = {
            { "Name", Q_ARG(QString, info.name), true, 0 },
            { "DiskName", Q_ARG(QString, info.diskName), true, 0 },
            { "Cover", Q_ARG(QPixmap, info.cover), false, HAS_COVER },
            { "Genre", Q_ARG(QString, info.genre), false, HAS_GENRE },
            { "Developer", Q_ARG(QString, info.developer), false, HAS_DEVELOPER },
            { "Publisher", Q_ARG(QString, info.publisher), false, HAS_PUBLISHER },
            { "Year", Q_ARG(int, info.year), false, HAS_YEAR }
        };

        for (unsigned i = 0; i < (sizeof(fields) / sizeof(struct Field)); i++) {
            if (!readField(inf, dirName, fields[i].fieldName, fields[i].fieldValue)) {
                if (fields[i].required) {
//                    qDebug() << "Required field: " << fields[i].fieldName;
                    return;
                }
            } else {
                info.options |= fields[i].option;
            }
        }

        emit gameLoaded(info);
        inf.endGroup();
    }
}

bool GameLoader::readField(const QSettings& inf, const QString& dirName, const QString& fieldName, QGenericArgument fieldValue) {
    QVariant var = inf.value(fieldName);

    int typeId = QMetaType::type(fieldValue.name());
    switch (typeId) {
        case QMetaType::QString:
            {
                QString tmp = var.value<QString>();
                if (tmp.isEmpty())
                    return false;

                QMetaType::construct(typeId, fieldValue.data(), &tmp);
            }

            break;
        case QMetaType::QPixmap:
            {
                QPixmap tmp(QString("%1/%2/%3").arg(m_root.absolutePath()).arg(dirName).arg(var.value<QString>()));
                if (tmp.isNull()) {
                    tmp = QPixmap(":/DefaultCover");
                } else if (tmp.size() != DefaultCoverSize) {
                    tmp = tmp.scaled(DefaultCoverSize);
                }

                Q_ASSERT(!tmp.isNull());

                QMetaType::construct(typeId, fieldValue.data(), &tmp);
            }

            break;
        case QMetaType::Int:
            {
                int tmp = var.value<int>();
                if (tmp <= 0)
                    return false;

                QMetaType::construct(typeId, fieldValue.data(), &tmp);
            }

            break;
        default:
            return false;
    }

    return true;
}
