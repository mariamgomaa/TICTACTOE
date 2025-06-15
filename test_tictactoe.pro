QT = core sql
CONFIG += console
CONFIG -= app_bundle

TARGET = test_tictactoe
TEMPLATE = app

SOURCES += test_tictactoe.cpp \
           tictactoegame.cpp \
           user.cpp \
           usermanager.cpp \
           $$PWD/googletest/googletest/src/gtest-all.cc

HEADERS += tictactoegame.h \
           user.h \
           usermanager.h

INCLUDEPATH += $$PWD/googletest/googletest/include \
               $$PWD/googletest/googletest

LIBS += -lpthread
