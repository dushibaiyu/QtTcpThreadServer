#include "common.h"
#include <errno.h>
#include "tco.h"

#if QT_VERSION >= 0x040400
#	include <QtCore/QAtomicInt>
#endif

#if defined(Q_OS_LINUX) && defined(O_CLOEXEC)
#	define THREADSAFE_CLOEXEC_SUPPORTED 1
namespace libcsupplement {
	inline int pipe2(int[], int) { errno = ENOSYS; return -1; }
}

using namespace libcsupplement;
#else
#	define THREADSAFE_CLOEXEC_SUPPORTED 0
#endif

class Q_DECL_HIDDEN ThreadCommunicationObjectPrivate {
public:
	ThreadCommunicationObjectPrivate(void)
		: fd(), isvalid(false)
#if QT_VERSION >= 0x040400
		  , wakeups()
#endif
	{
		this->fd[0] = -1;
		this->fd[1] = -1;

		int res;

#if defined(Q_OS_INTEGRITY) || defined(Q_OS_VXWORKS)
		if (::socketpair(AF_LOCAL, SOCK_STREAM, 0, this->fd)) {
			qErrnoWarning("%s: Failed to create a socket pair", Q_FUNC_INFO);
			return -1;
		}
#else
#	if THREADSAFE_CLOEXEC_SUPPORTED
		res = ::pipe2(this->fd, O_CLOEXEC | O_NONBLOCK);
		if (res != -1 && errno != ENOSYS) {
			if (-1 == res) {
				qErrnoWarning("%s: Failed to create a pipe", Q_FUNC_INFO);
			}

			this->isvalid = (res != -1);
			return;
		}
#	endif // THREADSAFE_CLOEXEC_SUPPORTED

		res = ::pipe(this->fd);
		if (-1 == res) {
			qErrnoWarning("%s: Failed to create a pipe", Q_FUNC_INFO);
			return;
		}
#endif // defined(Q_OS_INTEGRITY) || defined(Q_OS_VXWORKS)

		if (-1 == this->set_cloexec()) {
			qErrnoWarning("%s: Unable to set close on exec flag", Q_FUNC_INFO);
		}

		if (-1 == this->make_nonblocking()) {
			qErrnoWarning("%s: Unable to make the descriptor non-blocking", Q_FUNC_INFO);
		}

		this->isvalid = true;
	}

	~ThreadCommunicationObjectPrivate(void)
	{
		if (this->isvalid) {
			::close(this->fd[0]);
			::close(this->fd[1]);
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
			char val = 1;
			do {
				res = ::write(this->fd[1], &val, sizeof(val));
			} while (Q_UNLIKELY(res == -1 && errno == EINTR));

			if (Q_UNLIKELY(-1 == res)) {
				qErrnoWarning("%s: write() failed", Q_FUNC_INFO);
				return false;
			}
		}

		return true;
	}

	bool awaken(void)
	{
		Q_ASSERT(this->isvalid);

		char buf[16];
		int res;
		do {
			do {
				res = ::read(this->fd[0], buf, sizeof(buf));
			} while (Q_UNLIKELY(-1 == res && EINTR == errno));
		} while (res == sizeof(buf));

		if (Q_UNLIKELY(-1 == res)) {
			qErrnoWarning("%s: read() failed", Q_FUNC_INFO);
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
	qintptr readfd(void) const { return this->fd[0]; }

private:
	int fd[2];
	bool isvalid;
#if QT_VERSION >= 0x040400
	QAtomicInt wakeups;
#endif

	int set_cloexec(void)
	{
		int res = ::fcntl(this->fd[0], F_SETFD, FD_CLOEXEC);
		if (res != -1) {
			res = ::fcntl(this->fd[1], F_SETFD, FD_CLOEXEC);
		}

		return res;
	}

	int make_nonblocking(void)
	{
		int res = ::fcntl(this->fd[0], F_SETFL, O_NONBLOCK | ::fcntl(this->fd[0], F_GETFL));
		if (res != -1) {
			res = ::fcntl(this->fd[1], F_SETFL, O_NONBLOCK | ::fcntl(this->fd[1], F_GETFL));
		}

		return res;
	}
};

#undef THREADSAFE_CLOEXEC_SUPPORTED

#include "tco_impl.h"
