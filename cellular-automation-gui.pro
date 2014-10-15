#-------------------------------------------------
#
# Project created by QtCreator 2014-10-14T10:03:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cellular-automation-gui
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        Cell.cpp \
        CellMatrix.cpp \
    mainwindow.cpp \
    calthread.cpp

HEADERS  += \ 
        Cell.h \
        CellMatrix.h \
    config.h \
    mainwindow.h \
    calthread.h
        
unix|macx|win32: LIBS += -lgamcs
win32: LIBS += -lsqlite3    # windows still needs sqlite3.lib?
