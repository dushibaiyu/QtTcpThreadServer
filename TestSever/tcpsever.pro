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
    threadhandle.cpp \
    pinyinconvert.cpp

HEADERS += \
    mytcpserver.h \
    mytcpsocket.h \
    threadhandle.h \
    pinyinconvert.h
