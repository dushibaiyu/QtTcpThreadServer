#include "common.h"
#include <sys/eventfd.h>
#include <errno.h>
#include "tco.h"

#if QT_VERSION >= 0x040400
#	include <QtCore/QAtomicInt>
#endif

#if defined(EFD_CLOEXEC) && defined(EFD_NONBLOCK)
#	define MY_EFD_CLOEXEC EFD_CLOEXEC
#	define MY_EFD_NONBLOCK EFD_NONBLOCK
#else
#	define MY_EFD_CLOEXEC 0
#	define MY_EFD_NONBLOCK 0
#endif

class Q_DECL_HIDDEN ThreadCommunicationObjectPrivate {
public:
	ThreadCommunicationObjectPrivate(void)
		: fd(-1), isvalid(false)
#if QT_VERSION >= 0x040400
		  , wakeups()
#endif
	{
		int flags = MY_EFD_CLOEXEC | MY_EFD_NONBLOCK;
		int res   = ::eventfd(0, flags);
		this->fd  = -1;

		if (-1 == res) {
			if (EINVAL == errno && flags) {
				res = ::eventfd(0, 0);
			}

			if (res == -1) {
				qErrnoWarning("%s: eventfd() failed", Q_FUNC_INFO);
				return;
			}

			if (-1 == this->set_cloexec()) {
				qErrnoWarning("%s: Unable to set close on exec flag", Q_FUNC_INFO);
			}

			if (-1 == this->make_nonblocking()) {
				qErrnoWarning("%s: Unable to make the descriptor non-blocking", Q_FUNC_INFO);
			}
		}

		this->fd      = res;
		this->isvalid = true;
	}

	~ThreadCommunicationObjectPrivate(void)
	{
		if (this->fd != -1) {
			::close(this->fd);
		}
	}

	bool wakeUp(void)
	{
		Q_ASSERT(this->isvalid);

#if QT_VERSION >= 0x040400
		if (this->wakeups.testAndSetAcquire(0, 1))
#endif
		{
			int res;
			eventfd_t val = 1;
			do {
				res = eventfd_write(this->fd, val);
			} while (Q_UNLIKELY(res == -1 && errno == EINTR));

			if (Q_UNLIKELY(-1 == res)) {
				qErrnoWarning("%s: eventfd_write() failed", Q_FUNC_INFO);
				return false;
			}
		}

		return true;
	}

	bool awaken(void)
	{
		Q_ASSERT(this->isvalid);

		eventfd_t value;
		int res;
		do {
			res = eventfd_read(this->fd, &value);
		} while (Q_UNLIKELY(-1 == res && EINTR == errno));

		if (Q_UNLIKELY(-1 == res)) {
			qErrnoWarning("%s: eventfd_read() failed", Q_FUNC_INFO);
		}

#if QT_VERSION >= 0x040400
		if (Q_UNLIKELY(!this->wakeups.testAndSetRelease(1, 0))) {
			qCritical("%s: internal error, testAndSetRelease(1, 0) failed!", Q_FUNC_INFO);
			res = -1;
		}
#endif

		return res != -1;
	}

	bool valid(void) const { return this->isvalid; }
	qintptr readfd(void) const { return this->fd; }

private:
	int fd;
	bool isvalid;
#if QT_VERSION >= 0x040400
	QAtomicInt wakeups;
#endif

	int set_cloexec(void)
	{
		return ::fcntl(this->fd, F_SETFD, FD_CLOEXEC);
	}

	int make_nonblocking(void)
	{
		return ::fcntl(this->fd, F_SETFL, O_NONBLOCK | ::fcntl(this->fd, F_GETFL));
	}
};

#undef MY_EFD_CLOEXEC
#undef MY_EFD_NONBLOCK

#include "tco_impl.h"
