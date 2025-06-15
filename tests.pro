# tests.pro

QT -= gui
CONFIG += console c++17
CONFIG -= app_bundle
TEMPLATE = app
TARGET = TicTacToeTests

INCLUDEPATH += \
    $$PWD/googletest/googletest/include \
    $$PWD/googletest/googlemock/include

LIBS += -L$$PWD/googletest/build/lib -lgtest -lgtest_main


SOURCES += test_tictactoegame.cpp \
           tictactoegame.cpp \
           user.cpp \
           usermanager.cpp

HEADERS += \
    tictactoegame.h \
    user.h \
    usermanager.h
