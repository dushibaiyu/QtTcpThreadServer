#include "common.h"
#include "eventdispatcher_libevent.h"
#include "eventdispatcher_libevent_p.h"

/**
 * @class EventDispatcherLibEvent
 * @brief The EventDispatcherLibEvent class provides an interface to manage Qt's event queue.
 *
 * An event dispatcher receives events from the window system and other
 * sources. It then sends them to the QCoreApplication or QApplication
 * instance for processing and delivery. EventDispatcherLibEvent provides
 * fine-grained control over event delivery.
 *
 * For simple control of event processing use
 * QCoreApplication::processEvents().
 *
 * For finer control of the application's event loop, call
 * instance() and call functions on the QAbstractEventDispatcher
 * object that is returned.
 *
 * To use EventDispatcherLibEvent, you must install it with
 * QCoreApplication::setEventDispatcher() or QThread::setEventDispatcher()
 * before a default event dispatcher has been installed.

 * The main event loop is started by calling
 * QCoreApplication::exec(), and stopped by calling
 * QCoreApplication::exit(). Local event loops can be created using
 * QEventLoop.
 *
 * Programs that perform long operations can call processEvents()
 * with a bitwise OR combination of various QEventLoop::ProcessEventsFlag
 * values to control which events should be delivered.
 */

/**
 * Constructs a new event dispatcher with the given @a parent
 *
 * @param parent Parent
 */
EventDispatcherLibEvent::EventDispatcherLibEvent(QObject* parent)
	: QAbstractEventDispatcher(parent), d_ptr(new EventDispatcherLibEventPrivate(this))
{
}

/**
 * Constructs a new event dispatcher with the given configuration @a config and @a parent
 *
 * @param config Event dispatcher configuration
 * @param parent Parent
 * @see EventDispatcherLibEventConfig
 */
EventDispatcherLibEvent::EventDispatcherLibEvent(const EventDispatcherLibEventConfig& config, QObject* parent)
	: QAbstractEventDispatcher(parent), d_ptr(new EventDispatcherLibEventPrivate(this, config))
{
}

/**
 * Destroys the event dispatcher
 */
EventDispatcherLibEvent::~EventDispatcherLibEvent(void)
{
#if QT_VERSION < 0x040600
	delete this->d_ptr;
	this->d_ptr = 0;
#endif
}

/**
 * @brief Processes pending events that match @a flags until there are no more events to process
 * @param flags
 * @return Whether an event has been processed
 * @retval true If an event has been processed;
 * @retval false If no events have been processed
 *
 * Processes pending events that match @a flags until there are no
 * more events to process. Returns true if an event was processed;
 * otherwise returns false.
 *
 * This function is especially useful if you have a long running
 * operation and want to show its progress without allowing user
 * input; i.e. by using the QEventLoop::ExcludeUserInputEvents flag.
 *
 * If the QEventLoop::WaitForMoreEvents flag is set in @a flags, the
 * behavior of this function is as follows:
 *
 * @list
 * @li If events are available, this function returns after processing them.
 * @li If no events are available, this function will wait until more
 * are available and return after processing newly available events.
 * @endlist
 *
 * If the QEventLoop::WaitForMoreEvents flag is not set in @a flags,
 * and no events are available, this function will return
 * immediately.
 *
 * @note This function does not process events continuously; it
 * returns after all available events are processed.
 *
 * @see hasPendingEvents()
 */
bool EventDispatcherLibEvent::processEvents(QEventLoop::ProcessEventsFlags flags)
{
	Q_D(EventDispatcherLibEvent);
	return d->processEvents(flags);
}

/**
 * Returns @c true if there is an event waiting; otherwise returns @c false
 *
 * @return Whether there is an event waiting
 */
bool EventDispatcherLibEvent::hasPendingEvents(void)
{
	extern uint qGlobalPostedEventsCount();
	return qGlobalPostedEventsCount() > 0;
}

/**
 * Registers @a notifier with the event loop
 *
 * @param notifier Socket notifier to register
 * @warning @a notifier must belong to the same thread as the event dispatcher
 * @warning Socket notifiers can only be enabled from the thread they belong to
 */
void EventDispatcherLibEvent::registerSocketNotifier(QSocketNotifier* notifier)
{
#ifndef QT_NO_DEBUG
	if (notifier->socket() < 0) {
		qWarning("QSocketNotifier: Internal error: sockfd < 0");
		return;
	}

	if (notifier->thread() != this->thread() || this->thread() != QThread::currentThread()) {
		qWarning("QSocketNotifier: socket notifiers cannot be enabled from another thread");
		return;
	}
#endif

	if (notifier->type() == QSocketNotifier::Exception) {
		return;
	}

	Q_D(EventDispatcherLibEvent);
	d->registerSocketNotifier(notifier);
}

/**
 * Unregisters @a notifier from the event dispatcher.
 *
 * @param notifier Socket notifier to unregister
 * @warning @a notifier must belong to the same thread as the event dispatcher
 * @warning Socket notifiers can only be disabled from the thread they belong to
 */
void EventDispatcherLibEvent::unregisterSocketNotifier(QSocketNotifier* notifier)
{
#ifndef QT_NO_DEBUG
	if (notifier->socket() < 0) {
		qWarning("QSocketNotifier: Internal error: sockfd < 0");
		return;
	}

	if (notifier->thread() != this->thread() || this->thread() != QThread::currentThread()) {
		qWarning("QSocketNotifier: socket notifiers cannot be disabled from another thread");
		return;
	}
#endif

	// Short circuit, we do not support QSocketNotifier::Exception
	if (notifier->type() == QSocketNotifier::Exception) {
		return;
	}

	Q_D(EventDispatcherLibEvent);
	d->unregisterSocketNotifier(notifier);
}

/**
 * Register a timer with the specified @a timerId, @a interval, and @a timerType
 * for the given @a object.
 *
 * @param timerId Timer ID
 * @param interval Timer interval (msec); must be greater than 0
 * @param timerType Timer type (not available in Qt 4)
 * @param object Object for which the timer is registered
 * @warning @a object must belong to the same thread as the event dispatcher
 * @warning Timers can only be started from the thread the event dispatcher lives in
 */
void EventDispatcherLibEvent::registerTimer(
	int timerId,
	int interval,
#if QT_VERSION >= 0x050000
	Qt::TimerType timerType,
#endif
	QObject* object
)
{
#ifndef QT_NO_DEBUG
	if (timerId < 1 || interval < 0 || !object) {
		qWarning("%s: invalid arguments", Q_FUNC_INFO);
		return;
	}

	if (object->thread() != this->thread() && this->thread() != QThread::currentThread()) {
		qWarning("%s: timers cannot be started from another thread", Q_FUNC_INFO);
		return;
	}
#endif

	Qt::TimerType type;
#if QT_VERSION >= 0x050000
	type = timerType;
#else
	type = Qt::CoarseTimer;
#endif

	Q_D(EventDispatcherLibEvent);
	d->registerTimer(timerId, interval, type, object);
}

/**
 * Unregisters the timer
 *
 * @param timerId ID of the timer
 * @return Whether the timer has been unregistered
 * @warning Timers can only be stopped from the same thread as the event dispatcher lives in
 */
bool EventDispatcherLibEvent::unregisterTimer(int timerId)
{
#ifndef QT_NO_DEBUG
	if (timerId < 1) {
		qWarning("%s: invalid arguments", Q_FUNC_INFO);
		return false;
	}

	if (this->thread() != QThread::currentThread()) {
		qWarning("%s: timers cannot be stopped from another thread", Q_FUNC_INFO);
		return false;
	}
#endif

	Q_D(EventDispatcherLibEvent);
	return d->unregisterTimer(timerId);
}

/**
 * Unregisters all timers for the given @a object
 *
 * @param object Object
 * @return Whether timers have been unregistered
 * @warning @a object must belong to the same thread as the event dispatcher
 * @warning Timers can only be started from the thread the event dispatcher lives in
 */
bool EventDispatcherLibEvent::unregisterTimers(QObject* object)
{
#ifndef QT_NO_DEBUG
	if (!object) {
		qWarning("%s: invalid arguments", Q_FUNC_INFO);
		return false;
	}

	if (object->thread() != this->thread() && this->thread() != QThread::currentThread()) {
		qWarning("%s: timers cannot be stopped from another thread", Q_FUNC_INFO);
		return false;
	}
#endif

	Q_D(EventDispatcherLibEvent);
	return d->unregisterTimers(object);
}

/**
 * @brief Returns a list of registered timers for the given @a object.
 * @param object Object
 * @return List of registered timers
 */
QList<QAbstractEventDispatcher::TimerInfo> EventDispatcherLibEvent::registeredTimers(QObject* object) const
{
	if (!object) {
		qWarning("%s: invalid argument", Q_FUNC_INFO);
		return QList<QAbstractEventDispatcher::TimerInfo>();
	}

	Q_D(const EventDispatcherLibEvent);
	return d->registeredTimers(object);
}

#if QT_VERSION >= 0x050000
/**
 * Returns the remaining time in milliseconds with the given @a timerId.
 *
 * @param timerId Timer ID
 * @return The remaining time
 * @retval -1 If the timer is inactive
 * @retval 0 If the timer is overdue
 */
int EventDispatcherLibEvent::remainingTime(int timerId)
{
	Q_D(const EventDispatcherLibEvent);
	return d->remainingTime(timerId);
}
#endif

#if defined(Q_OS_WIN) && QT_VERSION >= 0x050000
bool EventDispatcherLibEvent::registerEventNotifier(QWinEventNotifier* notifier)
{
	Q_UNUSED(notifier)
	Q_UNIMPLEMENTED();
	return false;
}

void EventDispatcherLibEvent::unregisterEventNotifier(QWinEventNotifier* notifier)
{
	Q_UNUSED(notifier)
	Q_UNIMPLEMENTED();
}
#endif

/**
 * Wakes up the event loop. Thread-safe
 */
void EventDispatcherLibEvent::wakeUp(void)
{
	Q_D(EventDispatcherLibEvent);
	d->m_tco->wakeUp();
}

/**
 * Interrupts event dispatching. The event dispatcher will return from @c processEvents()
 * as soon as possible.
 */
void EventDispatcherLibEvent::interrupt(void)
{
	Q_D(EventDispatcherLibEvent);
	d->m_interrupt = true;
	this->wakeUp();
}

/**
 * @brief Does nothing
 */
void EventDispatcherLibEvent::flush(void)
{
}

/**
 * @brief EventDispatcherLibEvent::EventDispatcherLibEvent
 * @param dd
 * @param parent
 * @internal
 */
EventDispatcherLibEvent::EventDispatcherLibEvent(EventDispatcherLibEventPrivate& dd, QObject* parent)
	: QAbstractEventDispatcher(parent), d_ptr(&dd)
{
}

void EventDispatcherLibEvent::reinitialize(void)
{
	Q_D(EventDispatcherLibEvent);
	event_reinit(d->m_base);
}

/**
 * @fn void EventDispatcherLibEvent::awake()
 *
 * This signal is emitted after the event loop returns from a function that could block.
 *
 * @see wakeUp()
 * @see aboutToBlock()
 */

/**
 * @fn void EventDispatcherLibEvent::aboutToBlock()
 *
 * This signal is emitted before the event loop calls a function that could block.
 *
 * @see awake()
 */
