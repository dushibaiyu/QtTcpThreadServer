#ifndef INCLUDEWIN_H
#define INCLUDEWIN_H

#if defined __cplusplus

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <qplatformdefs.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QEvent>
#include <QtCore/QHash>
#include <QtCore/QMultiHash>
#include <QtCore/QPair>
#include <QtCore/QPointer>
#include <QtCore/QSocketNotifier>
#include <QtCore/QThread>
#include "qt4compat.h"

#ifdef Q_OS_WIN
#include "wsainit.h"
#endif

#endif //defined(__cplusplus)

#if defined(SJ_LIBEVENT_MAJOR) && SJ_LIBEVENT_MAJOR == 1
#	include "libevent2-emul.h"
#else
#	include <event2/event.h>
#endif

#endif // INCLUDEWIN_H
