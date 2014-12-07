#-------------------------------------------------
#
# Project created by QtCreator 2013-09-16T14:24:31
#
#-------------------------------------------------

QT       += core network

TARGET = QThreadTcpServer
TEMPLATE = app

QT       -= gui
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += C++11

SOURCES += $$PWD/main.cpp\
    $$PWD/tcpserver.cpp \
    $$PWD/tcpsocket.cpp \
    $$PWD/threadhandle.cpp

HEADERS  += \
    $$PWD/tcpserver.h \
    $$PWD/tcpsocket.h \
    $$PWD/threadhandle.h

unix:include($$PWD/eventdispatcher_libev/eventdispatcher_libev.pri)
