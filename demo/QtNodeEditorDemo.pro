#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T21:51:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtNodeEditorDemo
TEMPLATE = app

INCLUDEPATH += ../include

SOURCES += main.cpp\
    ../src/graphlink.cpp \
    ../src/graphnode.cpp \
    ../src/graphnodefield.cpp \
    ../src/graphscene.cpp \
    ../src/graphslot.cpp \
    nodeeditor.cpp

HEADERS  += \
    ../include/graphlink.h \
    ../include/graphnode.h \
    ../include/graphnodefield.h \
    ../include/graphscene.h \
    ../include/graphslot.h \
    ../include/graphtypes.h \
    nodeeditor.h

FORMS    +=
