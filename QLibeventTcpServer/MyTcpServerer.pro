#-------------------------------------------------
#
# Project created by QtCreator 2013-09-16T14:24:31
#
#-------------------------------------------------

QT       += core network

TARGET = QLibeventTcpServerer
TEMPLATE = app

QT       -= gui
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += C++11

SOURCES += main.cpp\
    mytcpserver.cpp \
    mytcpsocket.cpp \
    threadhandle.cpp \
    eventdispatcher_libevent/eventdispatcher_libevent.cpp \
    eventdispatcher_libevent/eventdispatcher_libevent_config.cpp \
    eventdispatcher_libevent/eventdispatcher_libevent_p.cpp \
    eventdispatcher_libevent/socknot_p.cpp \
    eventdispatcher_libevent/timers_p.cpp \
    pinyinconvert.cpp

HEADERS  += \
    mytcpserver.h \
    mytcpsocket.h \
    threadhandle.h \
    eventdispatcher_libevent/common.h \
    eventdispatcher_libevent/eventdispatcher_libevent.h \
    eventdispatcher_libevent/eventdispatcher_libevent_config.h \
    eventdispatcher_libevent/eventdispatcher_libevent_config_p.h \
    eventdispatcher_libevent/eventdispatcher_libevent_p.h \
    eventdispatcher_libevent/libevent2-emul.h \
    eventdispatcher_libevent/qt4compat.h \
    eventdispatcher_libevent/tco.h \
    eventdispatcher_libevent/tco_impl.h \
    pinyinconvert.h

FORMS    +=

unix {
        CONFIG += create_pc

        system('cc -E $$PWD/conftests/eventfd.h -o /dev/null 2> /dev/null') {
                SOURCES += eventdispatcher_libevent/tco_eventfd.cpp
        }
        else {
                SOURCES += eventdispatcher_libevent/tco_pipe.cpp
        }

        system('pkg-config --exists libevent') {
                CONFIG    += link_pkgconfig
                PKGCONFIG += libevent
        }
        else {
                system('cc -E $$PWD/conftests/libevent2.h -o /dev/null 2> /dev/null') {
                        DEFINES += SJ_LIBEVENT_MAJOR=2
                }
                else:system('cc -E $$PWD/conftests/libevent1.h -o /dev/null 2> /dev/null') {
                        DEFINES += SJ_LIBEVENT_MAJOR=1
                }
                else {
                        warning("Assuming libevent 1.x")
                        DEFINES += SJ_LIBEVENT_MAJOR=1
                }

                LIBS += -levent_core
        }
}

win32 {
        SOURCES += eventdispatcher_libevent/tco_win32_libevent.cpp
        HEADERS += eventdispatcher_libevent/wsainit.h
        LIBS    += $$QMAKE_LIBS_NETWORK
        LIBS    += -LE:/Libs/libevent/ -llibevent

        INCLUDEPATH += E:/Libs/libevent
        DEPENDPATH += E:/Libs/libevent
}

