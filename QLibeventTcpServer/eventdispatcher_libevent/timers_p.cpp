#include "common.h"
#include "eventdispatcher_libevent_p.h"

void EventDispatcherLibEventPrivate::calculateCoarseTimerTimeout(TimerInfo* info, const struct timeval& now, struct timeval& when)
{
	Q_ASSERT(info->interval > 20);
	// The coarse timer works like this:
	//  - interval under 40 ms: round to even
	//  - between 40 and 99 ms: round to multiple of 4
	//  - otherwise: try to wake up at a multiple of 25 ms, with a maximum error of 5%
	//
	// We try to wake up at the following second-fraction, in order of preference:
	//    0 ms
	//  500 ms
	//  250 ms or 750 ms
	//  200, 400, 600, 800 ms
	//  other multiples of 100
	//  other multiples of 50
	//  other multiples of 25
	//
	// The objective is to make most timers wake up at the same time, thereby reducing CPU wakeups.

	int interval     = info->interval;
	int msec         = static_cast<int>(info->when.tv_usec / 1000);
	int max_rounding = interval / 20; // 5%
	when             = info->when;

	if (interval < 100 && (interval % 25) != 0) {
		if (interval < 50) {
			uint round_up = ((msec % 50) >= 25) ? 1 : 0;
			msec = ((msec >> 1) | round_up) << 1;
		}
		else {
			uint round_up = ((msec % 100) >= 50) ? 1 : 0;
			msec = ((msec >> 2) | round_up) << 2;
		}
	}
	else {
		int min = qMax(0, msec - max_rounding);
		int max = qMin(1000, msec + max_rounding);

		bool done = false;

		// take any round-to-the-second timeout
		if (min == 0) {
			msec = 0;
			done = true;
		}
		else if (max == 1000) {
			msec = 1000;
			done = true;
		}

		if (!done) {
			int boundary;

			// if the interval is a multiple of 500 ms and > 5000 ms, always round towards a round-to-the-second
			// if the interval is a multiple of 500 ms, round towards the nearest multiple of 500 ms
			if ((interval % 500) == 0) {
				if (interval >= 5000) {
					msec = msec >= 500 ? max : min;
					done = true;
				}
				else {
					boundary = 500;
				}
			}
			else if ((interval % 50) == 0) {
				// same for multiples of 250, 200, 100, 50
				uint tmp = interval / 50;
				if ((tmp % 4) == 0) {
					boundary = 200;
				}
				else if ((tmp % 2) == 0) {
					boundary = 100;
				}
				else if ((tmp % 5) == 0) {
					boundary = 250;
				}
				else {
					boundary = 50;
				}
			}
			else {
				boundary = 25;
			}

			if (!done) {
				int base   = (msec / boundary) * boundary;
				int middle = base + boundary / 2;
				msec       = (msec < middle) ? qMax(base, min) : qMin(base + boundary, max);
			}
		}
	}

	if (msec == 1000) {
		++when.tv_sec;
		when.tv_usec = 0;
	}
	else {
		when.tv_usec = msec * 1000;
	}

	if (evutil_timercmp(&when, &now, <)) {
		when.tv_sec  += interval / 1000;
		when.tv_usec += (interval % 1000) * 1000;
		if (when.tv_usec > 999999) {
			++when.tv_sec;
			when.tv_usec -= 1000000;
		}
	}

	Q_ASSERT(evutil_timercmp(&now, &when, <=));
}

void EventDispatcherLibEventPrivate::calculateNextTimeout(TimerInfo* info, const struct timeval& now, struct timeval& delta)
{
	struct timeval tv_interval;
	struct timeval when;
	tv_interval.tv_sec  = info->interval / 1000;
	tv_interval.tv_usec = (info->interval % 1000) * 1000;

	if (info->interval) {
		qlonglong tnow  = (qlonglong(now.tv_sec)        * 1000) + (now.tv_usec        / 1000);
		qlonglong twhen = (qlonglong(info->when.tv_sec) * 1000) + (info->when.tv_usec / 1000);

		if ((info->interval < 1000 && twhen - tnow > 1500) || (info->interval >= 1000 && twhen - tnow > 1.2*info->interval)) {
			info->when = now;
		}
	}

	if (Qt::VeryCoarseTimer == info->type) {
		if (info->when.tv_usec >= 500000) {
			++info->when.tv_sec;
		}

		info->when.tv_usec = 0;
		info->when.tv_sec += info->interval / 1000;
		if (info->when.tv_sec <= now.tv_sec) {
			info->when.tv_sec = now.tv_sec + info->interval / 1000;
		}

		when = info->when;
	}
	else if (Qt::PreciseTimer == info->type) {
		if (info->interval) {
			evutil_timeradd(&info->when, &tv_interval, &info->when);
			if (evutil_timercmp(&info->when, &now, <)) {
				evutil_timeradd(&now, &tv_interval, &info->when);
			}

			when = info->when;
		}
		else {
			when = now;
		}
	}
	else {
		evutil_timeradd(&info->when, &tv_interval, &info->when);
		if (evutil_timercmp(&info->when, &now, <)) {
			evutil_timeradd(&now, &tv_interval, &info->when);
		}

		EventDispatcherLibEventPrivate::calculateCoarseTimerTimeout(info, now, when);
	}

	evutil_timersub(&when, &now, &delta);
}

void EventDispatcherLibEventPrivate::registerTimer(int timerId, int interval, Qt::TimerType type, QObject* object)
{
	struct timeval now;
	evutil_gettimeofday(&now, 0);

	TimerInfo* info = new TimerInfo;
	info->self      = this;
	info->ev        = event_new(this->m_base, -1, 0, EventDispatcherLibEventPrivate::timer_callback, info);
	info->timerId   = timerId;
	info->interval  = interval;
	info->type      = type;
	info->object    = object;
	info->when      = now; // calculateNextTimeout() will take care of info->when
	Q_CHECK_PTR(info->ev);

	if (Qt::CoarseTimer == type) {
		if (interval >= 20000) {
			info->type = Qt::VeryCoarseTimer;
		}
		else if (interval <= 20) {
			info->type = Qt::PreciseTimer;
		}
	}

	struct timeval delta;
	EventDispatcherLibEventPrivate::calculateNextTimeout(info, now, delta);

	event_add(info->ev, &delta);
	this->m_timers.insert(timerId, info);
}

bool EventDispatcherLibEventPrivate::unregisterTimer(int timerId)
{
	TimerHash::Iterator it = this->m_timers.find(timerId);
	if (it != this->m_timers.end()) {
		TimerInfo* info = it.value();
		event_del(info->ev);
		event_free(info->ev);
		delete info;
		this->m_timers.erase(it);
		return true;
	}

	return false;
}

bool EventDispatcherLibEventPrivate::unregisterTimers(QObject* object)
{
	TimerHash::Iterator it = this->m_timers.begin();
	while (it != this->m_timers.end()) {
		TimerInfo* info = it.value();
		if (object == info->object) {
			event_del(info->ev);
			event_free(info->ev);
			delete info;
			it = this->m_timers.erase(it);
		}
		else {
			++it;
		}
	}

	return true;
}

QList<QAbstractEventDispatcher::TimerInfo> EventDispatcherLibEventPrivate::registeredTimers(QObject* object) const
{
	QList<QAbstractEventDispatcher::TimerInfo> res;

	TimerHash::ConstIterator it = this->m_timers.constBegin();
	while (it != this->m_timers.constEnd()) {
		TimerInfo* info = it.value();
		if (object == info->object) {
#if QT_VERSION < 0x050000
			QAbstractEventDispatcher::TimerInfo ti(it.key(), info->interval);
#else
			QAbstractEventDispatcher::TimerInfo ti(it.key(), info->interval, info->type);
#endif
			res.append(ti);
		}

		++it;
	}

	return res;
}

int EventDispatcherLibEventPrivate::remainingTime(int timerId) const
{
	TimerHash::ConstIterator it = this->m_timers.find(timerId);
	if (it != this->m_timers.end()) {
		const TimerInfo* info = it.value();
		struct timeval when;

		int r = event_pending(info->ev, EV_TIMEOUT, &when);
		if (r) {
			struct timeval now;
			evutil_gettimeofday(&now, 0);

			qulonglong tnow  = qulonglong(now.tv_sec)  * 1000000 + now.tv_usec;
			qulonglong twhen = qulonglong(when.tv_sec) * 1000000 + when.tv_usec;

			if (tnow > twhen) {
				return 0;
			}

			return static_cast<int>((twhen - tnow) / 1000);
		}
	}

	return -1;
}

void EventDispatcherLibEventPrivate::timer_callback(int fd, short int events, void* arg)
{
	Q_ASSERT(-1 == fd);
	Q_ASSERT(events & EV_TIMEOUT);
	Q_UNUSED(fd)
	Q_UNUSED(events)

	TimerInfo* info = reinterpret_cast<TimerInfo*>(arg);

	// Timer can be reactivated only after its callback finishes; processEvents() will take care of this
	PendingEvent event(info->object, new QTimerEvent(info->timerId));
	info->self->m_event_list.append(event);
}

bool EventDispatcherLibEventPrivate::disableTimers(bool disable)
{
	struct timeval now;
	if (!disable) {
		evutil_gettimeofday(&now, 0);
	}

	TimerHash::Iterator it = this->m_timers.begin();
	while (it != this->m_timers.end()) {
		TimerInfo* info = it.value();
		if (disable) {
			event_del(info->ev);
		}
		else {
			struct timeval delta;
			EventDispatcherLibEventPrivate::calculateNextTimeout(info, now, delta);
			event_add(info->ev, &delta);
		}

		++it;
	}

	return true;
}

void EventDispatcherLibEventPrivate::killTimers(void)
{
	if (!this->m_timers.isEmpty()) {
		TimerHash::Iterator it = this->m_timers.begin();
		while (it != this->m_timers.end()) {
			TimerInfo* info = it.value();
			event_del(info->ev);
			event_free(info->ev);
			delete info;
			++it;
		}

		this->m_timers.clear();
	}
}
