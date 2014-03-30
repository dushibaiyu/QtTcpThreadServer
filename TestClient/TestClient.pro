#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T08:44:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestClient
TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp \
    testsockets.cpp

HEADERS  += mainwindow.h \
    testsockets.h

FORMS    += mainwindow.ui
