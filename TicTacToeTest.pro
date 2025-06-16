TEMPLATE = app
CONFIG += console c++17
QT -= gui

# All source and test files are in root directory
SOURCES += test_tictactoe.cpp \
           tictactoegame.cpp

# Google Test include path
INCLUDEPATH += googletest/googletest/include

# Link against Google Test (core only)
LIBS += -Lbuild_gtest/Release -lgtest -pthread
