QT -= gui
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = test_tictactoe

SOURCES += test_tictactoe.cpp

# Path to GoogleTest installed by the action
SOURCES += $$PWD/googletest/googletest/src/gtest-all.cc

SOURCES += $$PWD/googletest/googlemock/src/gmock-all.cc


SOURCES += \
    test_tictactoe.cpp \
    tictactoegame.cpp \
    user.cpp \
    usermanager.cpp

HEADERS += \
    tictactoegame.h \
    user.h \
    usermanager.h
