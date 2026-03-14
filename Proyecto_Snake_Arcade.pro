TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Game.cpp \
        PantallaMenu.cpp \
        PantallaSeleccion.cpp \
        Renderer.cpp \
        Zerpent.cpp \
        main.cpp

HEADERS += \
    Game.h \
    PantallaMenu.h \
    PantallaSeleccion.h \
    Renderer.h \
    Zerpent.h

INCLUDEPATH += C:/SFML-2.6.2/include

LIBS += -LC:/SFML-2.6.2/lib \
        -lsfml-graphics \
        -lsfml-window \
        -lsfml-system

CONFIG += c++17
