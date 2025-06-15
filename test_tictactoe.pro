QT -= gui
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = test_tictactoe

SOURCES += test_tictactoe.cpp

# Path to GoogleTest installed by the action
INCLUDEPATH += $$PWD/googletest/googletest/include/gtest
INCLUDEPATH += $$PWD/googletest/googlemock/include/gmock

SOURCES += \
    test_tictactoe.cpp \
    tictactoegame.cpp \
    user.cpp \
    usermanager.cpp

HEADERS += \
    tictactoegame.h \
    user.h \
    usermanager.h
