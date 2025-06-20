QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = TicTacToe
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/tictactoegame.cpp \
    src/user.cpp \
    src/usermanager.cpp

HEADERS += \
    src/mainwindow.h \
    src/tictactoegame.h \
    src/user.h \
    src/usermanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
