#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Disable high DPI scaling to maintain consistent pixel-perfect size
    //QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    qRegisterMetaType<GameMove>("GameMove");
    qRegisterMetaType<GameRecord>("GameRecord");
    // Set fixed scale factor
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");
    qputenv("QT_SCALE_FACTOR", "1");

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
