#-------------------------------------------------
#
# Project created by QtCreator 2014-01-23T17:53:25
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = tcpsever
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp \
    mytcpserver.cpp \
    mytcpsocket.cpp \
    pinyinconvert.cpp \
    ../myQThreadTcpServer/threadhandle.cpp

HEADERS += \
    mytcpserver.h \
    mytcpsocket.h \
    pinyinconvert.h \
    ../myQThreadTcpServer/threadhandle.h
