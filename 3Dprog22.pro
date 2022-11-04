QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}



SOURCES += main.cpp \
    heightmap.cpp \
    light.cpp \
    meshgenerator.cpp \
    regularsurface.cpp \
    soundmanager.cpp \
    soundsource.cpp \
    sun.cpp \
    textfileinterface.cpp \
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
    visualobject.cpp \
    wavfilehandler.cpp

HEADERS += \
    OBJFileLoader.h \
    heightmap.h \
    light.h \
    meshgenerator.h \
    regularsurface.h \
    soundmanager.h \
    soundsource.h \
    sun.h \
    textfileinterface.h \
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
    visualobject.h \
    wavfilehandler.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    phongshader.frag \
    phongshader.vert \
    plainshader.frag \
    plainshader.vert \
    textureshader.frag \
    textureshader.vert
