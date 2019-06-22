TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        LSystems.cpp \
        main.cpp \
        sandpile.cpp
LIBS += -lsfml-audio
LIBS += -lsfml-graphics
LIBS += -lsfml-network
LIBS += -lsfml-window
LIBS += -lsfml-system

HEADERS += \
    LSystems.h
