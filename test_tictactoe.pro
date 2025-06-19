QT += core gui widgets sql testlib
CONFIG += console c++11
TARGET = test_tictactoe
TEMPLATE = app

SOURCES += test_tictactoe.cpp \
           tictactoegame.cpp \
           user.cpp \
           usermanager.cpp

HEADERS += tictactoegame.h \
           user.h \
           usermanager.h

# Link with Google Test
LIBS += -lgtest -lgtest_main -lpthread
