#-------------------------------------------------
#
# Project created by QtCreator 2013-09-16T14:24:31
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyTcpServerer
TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp \
    mytcpserver.cpp \
    mytcpsocket.cpp \
    threadhandle.cpp

HEADERS  += mainwindow.h \
    mytcpserver.h \
    mytcpsocket.h \
    threadhandle.h

FORMS    += mainwindow.ui


TRANSLATIONS = design.ts
