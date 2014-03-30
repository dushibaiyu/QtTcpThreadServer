#include "common.h"
#include "eventdispatcher_libevent_config.h"

#ifndef SJ_LIBEVENT_EMULATION
#	include "eventdispatcher_libevent_config_p.h"
#else
class EventDispatcherLibEventConfigPrivate {};
#endif

EventDispatcherLibEventConfig::EventDispatcherLibEventConfig(void)
	: d_ptr(new EventDispatcherLibEventConfigPrivate())
{
}

EventDispatcherLibEventConfig::~EventDispatcherLibEventConfig(void)
{
#if QT_VERSION < 0x040600
	delete this->d_ptr;
	this->d_ptr = 0;
#endif
}

#ifdef SJ_LIBEVENT_EMULATION
bool EventDispatcherLibEventConfig::avoidMethod(const QLatin1String&) { return false; }
bool EventDispatcherLibEventConfig::requireFeatures(Features) { return false; }
bool EventDispatcherLibEventConfig::setConfiguration(Configuration) { return false; }
#else
bool EventDispatcherLibEventConfig::avoidMethod(const QLatin1String& method)
{
	Q_D(EventDispatcherLibEventConfig);
	return d->avoidMethod(method.latin1());
}

bool EventDispatcherLibEventConfig::requireFeatures(Features f)
{
	int features = 0;

	if (f & EventDispatcherLibEventConfig::ev_ET) {
		features |= EV_FEATURE_ET;
	}

	if (f & EventDispatcherLibEventConfig::ev_O1) {
		features |= EV_FEATURE_O1;
	}

	if (f & EventDispatcherLibEventConfig::ev_FDs) {
		features |= EV_FEATURE_FDS;
	}

	Q_D(EventDispatcherLibEventConfig);
	return d->requireFeatures(features);
}

bool EventDispatcherLibEventConfig::setConfiguration(Configuration cfg)
{
	int config = 0;

	if (cfg & EventDispatcherLibEventConfig::cfg_NoLock) {
		config |= EVENT_BASE_FLAG_NOLOCK;
	}

	if (cfg & EventDispatcherLibEventConfig::cfg_IgnoreEnvironment) {
		config |= EVENT_BASE_FLAG_IGNORE_ENV;
	}

	if (cfg & EventDispatcherLibEventConfig::cfg_StartupIOCP) {
		config |= EVENT_BASE_FLAG_STARTUP_IOCP;
	}

	if (cfg & EventDispatcherLibEventConfig::cfg_NoCacheTime) {
		config |= EVENT_BASE_FLAG_NO_CACHE_TIME;
	}

	if (cfg & EventDispatcherLibEventConfig::cfg_EPollChangelist) {
		config |= EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST;
	}

	Q_D(EventDispatcherLibEventConfig);
	return d->setConfiguration(config);
}


EventDispatcherLibEventConfigPrivate::EventDispatcherLibEventConfigPrivate(void)
	: m_cfg(0)
{
	this->m_cfg = event_config_new();
	Q_CHECK_PTR(this->m_cfg);
}

EventDispatcherLibEventConfigPrivate::~EventDispatcherLibEventConfigPrivate(void)
{
	if (this->m_cfg) {
		event_config_free(this->m_cfg);
		this->m_cfg = 0;
	}
}

bool EventDispatcherLibEventConfigPrivate::avoidMethod(const char* method)
{
	return 0 == event_config_avoid_method(this->m_cfg, method);
}

bool EventDispatcherLibEventConfigPrivate::requireFeatures(int features)
{
	return 0 == event_config_require_features(this->m_cfg, features);
}

bool EventDispatcherLibEventConfigPrivate::setConfiguration(int config)
{
	return 0 == event_config_set_flag(this->m_cfg, config);
}

#endif
