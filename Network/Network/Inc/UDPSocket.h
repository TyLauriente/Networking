#pragma once

#include "SocketAddress.h"

namespace Network
{

class UDPSocket
{
public:
	UDPSocket();
	~UDPSocket();

	bool Open();
	void Close();

	bool Bind(const SocketAddress& fromAddress);
	bool SetBroadcast(bool broadcast);

	int SendTo(const void* buffer, int len, const SocketAddress& toAddress);
	int ReceiveFrom(void* buffer, int len, SocketAddress& fromAddress);

private:
	UDPSocket(const UDPSocket&) = delete;
	UDPSocket& operator=(const UDPSocket&) = delete;

	SOCKET m_socket;
};

} // namespace Network