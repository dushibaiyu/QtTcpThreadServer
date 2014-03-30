#include "common.h"

#if QT_VERSION >= 0x040400
#	include <QtCore/QAtomicInt>
#endif

class Q_DECL_HIDDEN ThreadCommunicationObjectPrivate {
public:
	ThreadCommunicationObjectPrivate(void)
		: fd(), isvalid(false)
#if QT_VERSION >= 0x040400
		  , wakeups()
#endif
	{
		this->fd[0] = INVALID_SOCKET;
		this->fd[1] = INVALID_SOCKET;

		if (0 != evutil_socketpair(AF_INET, SOCK_STREAM, 0, this->fd)) {
			qFatal("%s: evutil_socketpair() failed: %d", Q_FUNC_INFO, WSAGetLastError());
		}

		evutil_make_socket_nonblocking(this->fd[0]);
		evutil_make_socket_nonblocking(this->fd[1]);

		this->isvalid = true;
	}

	~ThreadCommunicationObjectPrivate(void)
	{
		if (SOCKET(this->fd[0]) != INVALID_SOCKET) {
			::closesocket(this->fd[0]);
			::closesocket(this->fd[1]);
		}
	}

	bool wakeUp(void)
	{
		Q_ASSERT(this->isvalid);

#if QT_VERSION >= 0x040400
		if (this->wakeups.testAndSetAcquire(0, 1))
#endif
		{
			const char c = '.';
			int res      = ::send(this->fd[1], &c, 1, 0);

			if (Q_UNLIKELY(1 != res)) {
				qWarning("%s: send() failed: %d", Q_FUNC_INFO, WSAGetLastError());
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
			res = ::recv(this->fd[0], buf, sizeof(buf), 0);
		} while (res == sizeof(buf));

		if (Q_UNLIKELY(SOCKET_ERROR == res)) {
			qErrnoWarning("%s: recv() failed: %d", Q_FUNC_INFO, WSAGetLastError());
		}

#if QT_VERSION >= 0x040400
		if (Q_UNLIKELY(!this->wakeups.testAndSetRelease(1, 0))) {
			qCritical("%s: internal error, testAndSetRelease(1, 0) failed!", Q_FUNC_INFO);
			res = -1;
		}
#endif

		return res != SOCKET_ERROR;
	}

	bool valid(void) const { return this->isvalid; }
	qintptr readfd(void) const { return this->fd[0]; }

private:
	qintptr fd[2];
	bool isvalid;
#if QT_VERSION >= 0x040400
	QAtomicInt wakeups;
#endif
};

#include "tco_impl.h"
