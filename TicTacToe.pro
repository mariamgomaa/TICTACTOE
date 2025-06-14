QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = TicTacToe
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    tictactoegame.cpp \
    user.cpp \
    usermanager.cpp

HEADERS += \
    mainwindow.h \
    tictactoegame.h \
    user.h \
    usermanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
