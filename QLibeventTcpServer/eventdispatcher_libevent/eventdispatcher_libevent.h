#ifndef EVENTDISPATCHER_LIBEVENT_H
#define EVENTDISPATCHER_LIBEVENT_H

#include <QtCore/QAbstractEventDispatcher>

class EventDispatcherLibEventPrivate;
class EventDispatcherLibEventConfig;

class EventDispatcherLibEvent : public QAbstractEventDispatcher {
	Q_OBJECT
public:
	explicit EventDispatcherLibEvent(QObject* parent = 0);
	EventDispatcherLibEvent(const EventDispatcherLibEventConfig& config, QObject* parent = 0);
	virtual ~EventDispatcherLibEvent(void);

	virtual bool processEvents(QEventLoop::ProcessEventsFlags flags);
	virtual bool hasPendingEvents(void);

	virtual void registerSocketNotifier(QSocketNotifier* notifier);
	virtual void unregisterSocketNotifier(QSocketNotifier* notifier);

	virtual void registerTimer(
		int timerId,
		int interval,
#if QT_VERSION >= 0x050000
		Qt::TimerType timerType,
#endif
		QObject* object
	);

	virtual bool unregisterTimer(int timerId);
	virtual bool unregisterTimers(QObject* object);
	virtual QList<QAbstractEventDispatcher::TimerInfo> registeredTimers(QObject* object) const;
#if QT_VERSION >= 0x050000
	virtual int remainingTime(int timerId);
#endif

#if defined(Q_OS_WIN) && QT_VERSION >= 0x050000
	virtual bool registerEventNotifier(QWinEventNotifier* notifier);
	virtual void unregisterEventNotifier(QWinEventNotifier* notifier);
#endif

	virtual void wakeUp(void);
	virtual void interrupt(void);
	virtual void flush(void);

protected:
	EventDispatcherLibEvent(EventDispatcherLibEventPrivate& dd, QObject* parent = 0);

public:
	void reinitialize(void);

private:
	Q_DISABLE_COPY(EventDispatcherLibEvent)
	Q_DECLARE_PRIVATE(EventDispatcherLibEvent)
#if QT_VERSION >= 0x040600
	QScopedPointer<EventDispatcherLibEventPrivate> d_ptr;
#else
	EventDispatcherLibEventPrivate* d_ptr;
#endif
};

#endif // EVENTDISPATCHER_LIBEVENT_H
