#include <QApplication>
#include <QMetaType>

#include "mainwindow.h"
#include "user.h"  // include this so it knows GameRecord

int main(int argc, char* argv[]) {
  // Register custom types with Qt
  qRegisterMetaType<GameMove>("GameMove");
  qRegisterMetaType<GameRecord>("GameRecord");

  // Disable high DPI scaling to maintain consistent pixel-perfect size
  // QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

  // Set fixed scale factor
  qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");
  qputenv("QT_SCALE_FACTOR", "1");

  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  return app.exec();
}
