#ifndef EVENTDISPATCHER_LIBEVENT_QPA_H
#define EVENTDISPATCHER_LIBEVENT_QPA_H

#include "eventdispatcher_libevent.h"

#if QT_VERSION < 0x050000
#	error This code requires at least Qt 5
#endif

class EventDispatcherLibEventQPA : public EventDispatcherLibEvent {
	Q_OBJECT
public:
	explicit EventDispatcherLibEventQPA(QObject* parent = 0);
	virtual ~EventDispatcherLibEventQPA(void);

	bool processEvents(QEventLoop::ProcessEventsFlags flags) Q_DECL_OVERRIDE;
	bool hasPendingEvents(void) Q_DECL_OVERRIDE;
	void flush(void) Q_DECL_OVERRIDE;

private:
	Q_DISABLE_COPY(EventDispatcherLibEventQPA)
};

#endif // EVENTDISPATCHER_LIBEVENT_QPA_H
