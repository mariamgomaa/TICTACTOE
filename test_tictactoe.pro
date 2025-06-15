QT = core sql
CONFIG += console
CONFIG -= app_bundle

TARGET = test_tictactoe
TEMPLATE = app

SOURCES += test_tictactoe.cpp \
           tictactoegame.cpp \
           user.cpp \
           usermanager.cpp \
           $$PWD/googletest/googletest/src/gtest-all.cc \
           $$PWD/googletest/googlemock/src/gmock-all.cc

HEADERS += tictactoegame.h \
           user.h \
           usermanager.h

INCLUDEPATH += $$PWD/googletest/googletest/include \
               $$PWD/googletest/googlemock/include \
               $$PWD/googletest/googletest \
               $$PWD/googletest/googlemock

# Windows-specific fixes for MinGW
win32 {
    LIBS += -lws2_32 -lole32 -loleaut32 -luuid -lwinmm
    DEFINES += GTEST_OS_WINDOWS
}

# Remove conflicting pthread - let Qt handle threading
# LIBS += -lpthread  # Remove this line
