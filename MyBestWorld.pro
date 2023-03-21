#-------------------------------------------------
#
# Project created by QtCreator 2017-12-03T14:32:12
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyBestWorld
TEMPLATE = app

LIBS += -lopengl32 -lglu32

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        Main/main.cpp \
        Main/mainwindow.cpp \
        Main/glwidget.cpp \
    Graphics/BSpline.cpp \
    Graphics/display.cpp \
    Graphics/math3d.cpp \
    bubbles.cpp

HEADERS += \
        Main/mainwindow.h \
        Main/glwidget.h \
    Graphics/BSpline.h \
    Graphics/formats.h \
    Graphics/light.h \
    Graphics/math3d.h \
    Graphics/reflection.h \
    Graphics/shadow.h \
    Graphics/system.h \
    Graphics/texgen.h \
    Graphics/Vector.h \
    bubbles.h

FORMS += \
        Main/mainwindow.ui
