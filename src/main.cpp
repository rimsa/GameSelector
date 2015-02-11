#include <QApplication>
#include <GameSelector.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GameSelector w;
    Q_UNUSED(w);

    return a.exec();
}
