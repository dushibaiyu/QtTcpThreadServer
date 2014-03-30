#ifndef EVENTDISPATCHER_LIBEVENT_P_H
#define EVENTDISPATCHER_LIBEVENT_P_H

#include "common.h"
#include "tco.h"

class EventDispatcherLibEvent;
class EventDispatcherLibEventConfig;
class EventDispatcherLibEventPrivate;

struct SocketNotifierInfo {
	QSocketNotifier* sn;
	struct event* ev;
};

struct TimerInfo {
	EventDispatcherLibEventPrivate* self;
	QObject* object;
	struct event* ev;
	struct timeval when;
	int timerId;
	int interval;
	Qt::TimerType type;
};

Q_DECLARE_TYPEINFO(SocketNotifierInfo, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(TimerInfo, Q_PRIMITIVE_TYPE);

class Q_DECL_HIDDEN EventDispatcherLibEventPrivate {
public:
	EventDispatcherLibEventPrivate(EventDispatcherLibEvent* const q);
	EventDispatcherLibEventPrivate(EventDispatcherLibEvent* const q, const EventDispatcherLibEventConfig& cfg);
	~EventDispatcherLibEventPrivate(void);
	bool processEvents(QEventLoop::ProcessEventsFlags flags);
	void registerSocketNotifier(QSocketNotifier* notifier);
	void unregisterSocketNotifier(QSocketNotifier* notifier);
	void registerTimer(int timerId, int interval, Qt::TimerType type, QObject* object);
	bool unregisterTimer(int timerId);
	bool unregisterTimers(QObject* object);
	QList<QAbstractEventDispatcher::TimerInfo> registeredTimers(QObject* object) const;
	int remainingTime(int timerId) const;

	typedef QMultiHash<evutil_socket_t, SocketNotifierInfo> SocketNotifierHash;
	typedef QHash<int, TimerInfo*> TimerHash;
	typedef QPair<QPointer<QObject>, QEvent*> PendingEvent;
	typedef QList<PendingEvent> EventList;

private:
	Q_DISABLE_COPY(EventDispatcherLibEventPrivate)
	Q_DECLARE_PUBLIC(EventDispatcherLibEvent)
	EventDispatcherLibEvent* const q_ptr;

	bool m_interrupt;
	struct event_base* m_base;
	struct event* m_wakeup;
	ThreadCommunicationObject* m_tco;
	SocketNotifierHash m_notifiers;
	TimerHash m_timers;
	EventList m_event_list;
	bool m_awaken;

	void initialize(const EventDispatcherLibEventConfig* cfg);

	static void calculateCoarseTimerTimeout(TimerInfo* info, const struct timeval& now, struct timeval& when);
	static void calculateNextTimeout(TimerInfo* info, const struct timeval& now, struct timeval& delta);

	static void socket_notifier_callback(evutil_socket_t fd, short int events, void* arg);
	static void timer_callback(evutil_socket_t fd, short int events, void* arg);
	static void wake_up_handler(evutil_socket_t fd, short int events, void* arg);

	bool disableSocketNotifiers(bool disable);
	void killSocketNotifiers(void);
	bool disableTimers(bool disable);
	void killTimers(void);
};

#endif // EVENTDISPATCHER_LIBEVENT_P_H
