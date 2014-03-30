#include "common.h"
#include "eventdispatcher_libevent_p.h"

void EventDispatcherLibEventPrivate::registerSocketNotifier(QSocketNotifier* notifier)
{
	evutil_socket_t sockfd = notifier->socket();
	short int what;
	switch (notifier->type()) {
		case QSocketNotifier::Read:  what = EV_READ; break;
		case QSocketNotifier::Write: what = EV_WRITE; break;
		case QSocketNotifier::Exception: /// FIXME
			return;

		default:
			Q_ASSERT(false);
			return;
	}

	what |= EV_PERSIST;
	struct event* ev = event_new(this->m_base, sockfd, what, EventDispatcherLibEventPrivate::socket_notifier_callback, this);
	Q_CHECK_PTR(ev);
	event_add(ev, 0);

	SocketNotifierInfo data;
	data.sn = notifier;
	data.ev = ev;
	this->m_notifiers.insertMulti(sockfd, data);
}

void EventDispatcherLibEventPrivate::unregisterSocketNotifier(QSocketNotifier* notifier)
{
	evutil_socket_t sockfd = notifier->socket();
	SocketNotifierHash::Iterator it = this->m_notifiers.find(sockfd);
	while (it != this->m_notifiers.end() && it.key() == sockfd) {
		SocketNotifierInfo& data = it.value();
		if (data.sn == notifier) {
			event_del(data.ev);
			event_free(data.ev);
			it = this->m_notifiers.erase(it);
		}
		else {
			++it;
		}
	}
}

void EventDispatcherLibEventPrivate::socket_notifier_callback(int fd, short int events, void* arg)
{
	EventDispatcherLibEventPrivate* disp = reinterpret_cast<EventDispatcherLibEventPrivate*>(arg);
	SocketNotifierHash::Iterator it = disp->m_notifiers.find(fd);
	while (it != disp->m_notifiers.end() && it.key() == fd) {
		SocketNotifierInfo& data   = it.value();
		QSocketNotifier::Type type = data.sn->type();

		if ((QSocketNotifier::Read == type && (events & EV_READ)) || (QSocketNotifier::Write == type && (events & EV_WRITE))) {
			PendingEvent event(data.sn, new QEvent(QEvent::SockAct));
			disp->m_event_list.append(event);
		}

		++it;
	}
}

bool EventDispatcherLibEventPrivate::disableSocketNotifiers(bool disable)
{
	SocketNotifierHash::Iterator it = this->m_notifiers.begin();
	while (it != this->m_notifiers.end()) {
		SocketNotifierInfo& data = it.value();
		if (disable) {
			event_del(data.ev);
		}
		else {
			event_add(data.ev, 0);
		}

		++it;
	}

	return true;
}

void EventDispatcherLibEventPrivate::killSocketNotifiers(void)
{
	if (!this->m_notifiers.isEmpty()) {
		EventDispatcherLibEventPrivate::SocketNotifierHash::Iterator it = this->m_notifiers.begin();
		while (it != this->m_notifiers.end()) {
			SocketNotifierInfo& data = it.value();
			event_del(data.ev);
			event_free(data.ev);
			++it;
		}

		this->m_notifiers.clear();
	}
}
