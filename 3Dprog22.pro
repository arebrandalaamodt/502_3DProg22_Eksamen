QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    heightmap.cpp \
    light.cpp \
    meshgenerator.cpp \
    regularsurface.cpp \
    sun.cpp \
    texture.cpp \
    trophy.cpp \
    vertex.cpp \
    camera.cpp \
    interactiveobject.cpp \
    logger.cpp \
    mainwindow.cpp \
    npc.cpp \
    renderwindow.cpp \
    shader.cpp \
    trianglesurface.cpp \
    visualobject.cpp

HEADERS += \
    OBJFileLoader.h \
    heightmap.h \
    light.h \
    meshgenerator.h \
    regularsurface.h \
    sun.h \
    texture.h \
    trophy.h \
    vertex.h \
    camera.h \
    interactiveobject.h \
    logger.h \
    mainwindow.h \
    npc.h \
    renderwindow.h \
    shader.h \
    trianglesurface.h \
    visualobject.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    phongshader.frag \
    phongshader.vert \
    plainshader.frag \
    plainshader.vert \
    textureshader.frag \
    textureshader.vert
