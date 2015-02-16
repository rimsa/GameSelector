#include <QDebug>

#include <QApplication>
#include <GameSelector/GameSelector.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QDir root = (argc == 2 ? QDir(argv[1]) : QDir::current());
    if (!root.exists())
        qDebug() << "Invalid directory";

    GameSelector w(root);
    Q_UNUSED(w);

    return a.exec();
}
