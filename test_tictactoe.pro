QT -= gui
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = test_tictactoe

# Your test and source files
SOURCES += test_tictactoe.cpp \
           tictactoegame.cpp \
           user.cpp \
           usermanager.cpp

HEADERS += tictactoegame.h \
           user.h \
           usermanager.h

# GoogleTest paths
INCLUDEPATH += $$PWD/googletest/googletest/include \
               $$PWD/googletest/googlemock/include

# Build GoogleTest from source
SOURCES += $$PWD/googletest/googletest/src/gtest-all.cc

# Internal GoogleTest includes
INCLUDEPATH += $$PWD/googletest/googletest

# Threading support for Windows
LIBS += -lpthread
