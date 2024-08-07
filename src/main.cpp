#include <QDebug>

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <GameSelector/GameSelector.h>

void showInvalidDirectory() {
    QMessageBox::critical(0, "Game Selector",
                             "Invalid directory",
                             QMessageBox::Ok);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);


    QString rootDir;
    if (argc == 2) {
        rootDir = QString(argv[1]);
    } else {
        QDir current(qApp->applicationDirPath());
#ifdef Q_OS_MACOS
        current.cdUp();
        current.cdUp();
#endif

        rootDir = QFileDialog::getExistingDirectory(0, "Select game root folder", current.absolutePath());
    }

    if (rootDir.isEmpty()) {
        showInvalidDirectory();
        return -1;
    }

    QDir root(rootDir);
    if (!root.exists()) {
        showInvalidDirectory();
        return -1;
    }

    GameSelector w(root);
    Q_UNUSED(w);

    return a.exec();
}
