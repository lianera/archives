#-------------------------------------------------
#
# Project created by QtCreator 2016-12-07T17:35:28
#
#-------------------------------------------------

QT       += core gui
QT += datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shell
TEMPLATE = app
RC_FILE = gd.rc
INCLUDEPATH += "../3rdparty/include"\
            "../algorithms/"

SOURCES += main.cpp \
    mainwindow.cpp

HEADERS  += \
    mainwindow.h\
    ../algorithms/minimization.hpp

FORMS    +=

DISTFILES +=

RESOURCES += \
    res.qrc
