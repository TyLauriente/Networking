#include"Precompiled.h"
#include"TCPSocket.h"

using namespace Network;

TCPSocket::TCPSocket()
	: m_socket(INVALID_SOCKET)
{

}

TCPSocket::~TCPSocket()
{
	Close();
}

bool TCPSocket::Open()
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		return false;
	}
	return true;
}

void TCPSocket::Close()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

bool TCPSocket::Connect(const SocketAddress& address)
{
	if (m_socket == INVALID_SOCKET && !Open())
	{
		return false;
	}

	int error = connect(m_socket, &address.m_sockAddr, static_cast<int>(address.GetSize()));
	if (error == INVALID_SOCKET)
	{
		return false;
	}
	return true;
}

bool TCPSocket::Bind(const SocketAddress& address)
{
	if (m_socket == INVALID_SOCKET)
	{
		return false;
	}
	int error = bind(m_socket, &address.m_sockAddr, static_cast<int>(address.GetSize()));
	if (error == INVALID_SOCKET)
	{
		return false;
	}
	return true;
}

bool TCPSocket::Listen(int backLog)
{
	if (m_socket == INVALID_SOCKET)
	{
		return false;
	}
	int error = listen(m_socket, backLog);
	if (error == INVALID_SOCKET)
	{
		return false;
	}
	return true;
}

TCPSocket* TCPSocket::Accept(SocketAddress& fromAddress)
{
	TCPSocket* clientSock;
	clientSock->m_socket = accept(m_socket, &fromAddress.m_sockAddr, (int*)fromAddress.GetSize());
	if (clientSock->m_socket == INVALID_SOCKET)
	{
		delete clientSock;
		clientSock = nullptr;
		return nullptr;
	}
	return clientSock;
}

int TCPSocket::Send(const void* buffer, int len)
{
	return send(m_socket, (const char*)buffer, len, 0);
}

int TCPSocket::Receive(void* buffer, int len)
{
	return recv(m_socket, (char*)buffer, len, 0);
}