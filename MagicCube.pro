#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T18:59:38
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagicCube
TEMPLATE = app


SOURCES += main.cpp\
    magiccubeglwidget.cpp \
    singlecube.cpp \
    rotation.cpp \
    cubecolorset.cpp \
    magiccube.cpp \
    surface.cpp \
    universal.cpp \
    rubiccubesolver.cpp \
    setflashspeedwidget.cpp \
    tester.cpp

HEADERS  += magiccubeglwidget.h \
    singlecube.h \
    rotation.h \
    mymath.h \
    easy_vector3.h \
    cubecolorset.h \
    magiccube.h \
    surface.h \
    universal.h \
    rubiccubesolver.h \
    setflashspeedwidget.h \
    tester.h

RESOURCES += \
    graph.qrc

RC_ICONS = "Logo.ico"
