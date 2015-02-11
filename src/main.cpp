#include <QApplication>
#include <GameSelector.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GameSelector w(argc == 2 ? argv[1] : ".");

    Q_UNUSED(w);

    return a.exec();
}
