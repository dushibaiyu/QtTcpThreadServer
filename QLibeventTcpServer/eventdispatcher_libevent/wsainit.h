#ifndef WSAINIT_H
#define WSAINIT_H

#include "common.h"

Q_DECL_HIDDEN inline bool WSAInitialized(void)
{
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET && WSAGetLastError() == WSANOTINITIALISED) {
		return false;
	}

	::closesocket(s);
	return true;
}

class Q_DECL_HIDDEN WSAInitializer {
public:
	WSAInitializer(void)
		: m_success(false)
	{
		WORD wVersionRequested = MAKEWORD(2, 2);
		WSADATA wsaData;
		int err = WSAStartup(wVersionRequested, &wsaData);
		if (Q_LIKELY(!err)) {
			this->m_success = true;
		}
		else {
			qWarning("%s: WSA initialization failed: %d", Q_FUNC_INFO, err);
		}
	}

	~WSAInitializer(void)
	{
		if (Q_LIKELY(this->m_success)) {
			WSACleanup();
		}
	}
private:
	bool m_success;
};

#endif // WSAINIT_H
