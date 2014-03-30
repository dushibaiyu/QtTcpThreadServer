#include <qplatformdefs.h>
#include <qpa/qwindowsysteminterface.h>
#include <QtGui/QGuiApplication>
#include "eventdispatcher_libevent_qpa.h"

EventDispatcherLibEventQPA::EventDispatcherLibEventQPA(QObject* parent)
	: EventDispatcherLibEvent(parent)
{
}

EventDispatcherLibEventQPA::~EventDispatcherLibEventQPA(void)
{
}

bool EventDispatcherLibEventQPA::processEvents(QEventLoop::ProcessEventsFlags flags)
{
	bool sent_events = QWindowSystemInterface::sendWindowSystemEvents(flags);

	if (EventDispatcherLibEvent::processEvents(flags)) {
		return true;
	}

	return sent_events;
}

bool EventDispatcherLibEventQPA::hasPendingEvents(void)
{
	return EventDispatcherLibEvent::hasPendingEvents() || QWindowSystemInterface::windowSystemEventsQueued();
}

void EventDispatcherLibEventQPA::flush(void)
{
	if (qApp) {
		qApp->sendPostedEvents();
	}
}
