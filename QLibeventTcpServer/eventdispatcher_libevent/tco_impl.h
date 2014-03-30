#include "tco.h"

ThreadCommunicationObject::ThreadCommunicationObject(void)
	: d_ptr(new ThreadCommunicationObjectPrivate())
{
}

ThreadCommunicationObject::~ThreadCommunicationObject(void)
{
#if QT_VERSION < 0x040600
	delete this->d_ptr;
	this->d_ptr = 0;
#endif
}

bool ThreadCommunicationObject::valid(void) const
{
	Q_D(const ThreadCommunicationObject);
	return d->valid();
}

bool ThreadCommunicationObject::wakeUp(void)
{
	Q_D(ThreadCommunicationObject);
	return d->wakeUp();
}

bool ThreadCommunicationObject::awaken(void)
{
	Q_D(ThreadCommunicationObject);
	return d->awaken();
}

qintptr ThreadCommunicationObject::fd(void) const
{
	Q_D(const ThreadCommunicationObject);
	return d->readfd();
}
