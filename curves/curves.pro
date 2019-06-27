TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += lib
INCLUDEPATH += gui_glut

DEFINES += USE_SDL
LIBS += -lglut -l3ds -lGLU -lGL -lm -lSDL -lSDL_image

SOURCES += \
        Object.cpp \
        car3ds.cpp \
        colorrgb.cpp \
        controlpoint.cpp \
        gui_glut/OpenTextures.cpp \
        gui_glut/extra.cpp \
        gui_glut/gui.cpp \
        lib/Camera.cpp \
        lib/CameraDistante.cpp \
        lib/CameraJogo.cpp \
        lib/Desenha.cpp \
        lib/Vetor3D.cpp \
        lib/model3ds.cpp \
        main.cpp \
        modelo3ds.cpp \
        spline.cpp

HEADERS += \
    Object.h \
    car3ds.h \
    colorrgb.h \
    controlpoint.h \
    gui_glut/OpenTextures.h \
    gui_glut/extra.h \
    gui_glut/gui.h \
    lib/Camera.h \
    lib/CameraDistante.h \
    lib/CameraJogo.h \
    lib/Desenha.h \
    lib/Vetor3D.h \
    lib/model3ds.h \
    modelo3ds.h \
    spline.h
