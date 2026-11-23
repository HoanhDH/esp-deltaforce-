QT = core gui widgets
TEMPLATE = app
TARGET = khoang-khuat
CONFIG += c++17
win32-msvc*: QMAKE_CXXFLAGS += /utf-8
INCLUDEPATH += src desktop
SOURCES += desktop/player.cpp desktop/painting.cpp src/scene.cpp
HEADERS += desktop/painting.hpp src/canvas.hpp src/replay.hpp
