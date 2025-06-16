TEMPLATE = app
CONFIG += console c++17
QT -= gui

# All source and test files are in root directory
SOURCES += test_tictactoe.cpp \
           tictactoe.cpp

# Google Test include path
INCLUDEPATH += googletest/googletest/include

# Google Test build output directory
LIBS += -Lbuild_gtest/Release -lgtest -lgtest_main -pthread
