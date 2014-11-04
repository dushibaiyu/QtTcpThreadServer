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

SOURCES += main.cpp\
    tcpserver.cpp \
    tcpsocket.cpp \
    threadhandle.cpp 

HEADERS  += \
    tcpserver.h \
    tcpsocket.h \
    threadhandle.h 

