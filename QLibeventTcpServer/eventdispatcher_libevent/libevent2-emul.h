#ifndef EVENT2_EVENT_H
#define EVENT2_EVENT_H

#define SJ_LIBEVENT_EMULATION 1

#include <event.h>
#ifndef EV_H_
// libevent emulation by libev
#	include <evutil.h>
#endif
#include "qt4compat.h"

typedef int evutil_socket_t;
typedef void(*event_callback_fn)(evutil_socket_t, short, void*);

Q_DECL_HIDDEN inline struct event* event_new(struct event_base* base, evutil_socket_t fd, short int events, event_callback_fn callback, void* callback_arg)
{
	struct event* e = new struct event;
	event_set(e, fd, events, callback, callback_arg);
	event_base_set(base, e);
	return e;
}

Q_DECL_HIDDEN inline void event_free(struct event* e)
{
	delete e;
}

#ifdef EV_H_
Q_DECL_HIDDEN inline int event_reinit(struct event_base* base)
{
	Q_UNUSED(base);
	qWarning("%s emulation is not supported.", Q_FUNC_INFO);
	return 1;
}

#define evutil_gettimeofday(tv, tz)    gettimeofday((tv), (tz))
#define evutil_timeradd(tvp, uvp, vvp) timeradd((tvp), (uvp), (vvp))
#define evutil_timersub(tvp, uvp, vvp) timersub((tvp), (uvp), (vvp))
#define evutil_timercmp(tvp, uvp, cmp)        \
	(((tvp)->tv_sec == (uvp)->tv_sec) ?       \
		((tvp)->tv_usec cmp (uvp)->tv_usec) : \
		((tvp)->tv_sec cmp (uvp)->tv_sec))

#endif

#endif
