#pragma once

#include "SocketAddress.h"

namespace Network
{

class TCPSocket
{
public:
	TCPSocket();
	~TCPSocket();

	bool Open();
	void Close();

	bool Connect(const SocketAddress& address);
	bool Bind(const SocketAddress& address);
	bool Listen(int backLog = 10);
	TCPSocket* Accept(SocketAddress& fromAddress);

	int Send(const void* buffer, int len);
	int Receive(void* buffer, int len);

private:
	TCPSocket(const TCPSocket&) = delete;
	TCPSocket& operator=(const TCPSocket*) = delete;

	SOCKET m_socket;

};

} // namespace Network