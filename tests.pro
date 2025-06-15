QT -= gui
QT += core sql testlib

CONFIG += console
CONFIG += c++17
TEMPLATE = app
TARGET = TicTacToeTests

INCLUDEPATH += \
    $$PWD/googletest/googletest/include \
    $$PWD/googletest/googlemock/include

SOURCES += \
    test_tictactoegame.cpp \
    tictactoegame.cpp \
    user.cpp \
    usermanager.cpp

HEADERS += \
    tictactoegame.h \
    user.h \
    usermanager.h
