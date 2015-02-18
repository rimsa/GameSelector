#include <QDebug>

#include <QDir>
#include <QSettings>
#include <QGenericArgument>
#include <cstdlib>

#include <GameSelector/loader/GameLoader.h>
#include <GameSelector/util/GameInfo.h>

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
    QFile gameDesc(QString("%1%2%3%2game.inf")
            .arg(m_root.path())
            .arg(QDir::separator())
            .arg(dirName));
    if (!gameDesc.exists())
        return;

    struct GameInfo info;
    info.dirName = dirName;
    info.options = 0;

    QSettings inf(gameDesc.fileName(), QSettings::IniFormat, this);
    inf.beginGroup("Game");

    struct Field {
        QString fieldName;
        QGenericArgument fieldValue;
        bool required;
        GameInfoOptions option;
    } fields[] = {
        { "Name", Q_ARG(QString, info.name), true, 0 },
        { "DiskName", Q_ARG(QString, info.diskName), true, 0 },
        { "Cover", Q_ARG(QPixmap, info.cover), true, 0 },
        { "Arcade", Q_ARG(bool, info.arcade), true, 0 },
        { "Genre", Q_ARG(QString, info.genre), false, HAS_GENRE },
        { "Developer", Q_ARG(QString, info.developer), false, HAS_DEVELOPER },
        { "Publisher", Q_ARG(QString, info.publisher), false, HAS_PUBLISHER },
        { "Year", Q_ARG(int, info.year), false, HAS_YEAR },
        { "Size", Q_ARG(float, info.size), false, HAS_SIZE },
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

                // If the image is not loaded
                // the default black cover
                if (tmp.isNull()) {
                    tmp = QPixmap(":/DefaultCover");
                }

                // Scale the image if necessary.
                if (tmp.size() != DefaultCoverSize)
                    tmp = tmp.scaled(DefaultCoverSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

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
        case QMetaType::Float:
            {
                float tmp = var.value<float>();
                if (tmp <= 0.0)
                    return false;

                QMetaType::construct(typeId, fieldValue.data(), &tmp);
            }

            break;
        case QMetaType::Bool:
            {
                bool tmp = false;

                QString val = var.value<QString>();
                if (val.compare("yes", Qt::CaseInsensitive) == 0 ||
                    val.compare("true", Qt::CaseInsensitive) == 0 ||
                    val.compare("1", Qt::CaseInsensitive))
                    tmp = true;

                QMetaType::construct(typeId, fieldValue.data(), &tmp);
            }

            break;
        default:
            return false;
    }

    return true;
}
