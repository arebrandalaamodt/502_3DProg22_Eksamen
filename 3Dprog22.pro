QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    heightmap.cpp \
    light.cpp \
    meshgenerator.cpp \
    sun.cpp \
    texture.cpp \
    triangle.cpp \
    vertex.cpp \
    camera.cpp \
    graph.cpp \
    interactiveobject.cpp \
    logger.cpp \
    mainwindow.cpp \
    npc.cpp \
    renderwindow.cpp \
    shader.cpp \
    trianglesurface.cpp \
    visualobject.cpp

HEADERS += \
    heightmap.h \
    light.h \
    meshgenerator.h \
    sun.h \
    texture.h \
    triangle.h \
    vertex.h \
    camera.h \
    graph.h \
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
