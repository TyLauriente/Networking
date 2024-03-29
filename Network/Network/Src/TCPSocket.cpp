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

bool TCPSocket::SetNoDelay(bool noDelay)
{
	if (m_socket == INVALID_SOCKET && !Open())
	{
		return false;
	}
	int delayArg = noDelay ? 1 : 0;
	int result = setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&delayArg, sizeof(int));
	if (result == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool TCPSocket::SetNonBlocking(bool nonBlocking)
{
	if (m_socket == INVALID_SOCKET && !Open())
	{
		return false;
	}
	u_long arg = nonBlocking ? 1 : 0;
	int result = ioctlsocket(m_socket, FIONBIO, &arg);
	if (result == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool TCPSocket::Connect(const SocketAddress& address)
{
	if (m_socket == INVALID_SOCKET && !Open())
	{
		return false;
	}

	int result = connect(m_socket, &address.m_sockAddr, static_cast<int>(address.GetSize()));
	if (result == INVALID_SOCKET)
	{
		int lastError = WSAGetLastError();
		if (lastError == WSAEWOULDBLOCK)
		{
			fd_set write, err;
			FD_ZERO(&write);
			FD_ZERO(&err);
			FD_SET(m_socket, &write);
			FD_SET(m_socket, &err);

			TIMEVAL timeout = { 10, 0 }; // timeout after 10 seconds
			result = select(0, NULL, &write, &err, &timeout);
			if (result == 0)
			{
				// Timed out
				return false;
			}
			else
			{
				if (FD_ISSET(m_socket, &write))
				{
					return true;
				}
				if (FD_ISSET(m_socket, &err))
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool TCPSocket::Bind(const SocketAddress& address)
{
	if (m_socket == INVALID_SOCKET && !Open())
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
	socklen_t socketLength = (socklen_t)fromAddress.GetSize();
	SOCKET newSocket = accept(m_socket, &fromAddress.m_sockAddr, &socketLength);
	if (newSocket == INVALID_SOCKET)
	{
		return nullptr;
	}

	TCPSocket* newConnection = new TCPSocket();
	newConnection->m_socket = newSocket;
	return newConnection;
}

int TCPSocket::Send(const void* buffer, int len)
{
	return send(m_socket, (const char*)buffer, len, 0);
}

int TCPSocket::Receive(void* buffer, int len)
{
	int bytesRead = recv(m_socket, (char*)buffer, len, 0);
	if (bytesRead == SOCKET_ERROR)
	{
		int lastError = WSAGetLastError();
		if (lastError == WSAEWOULDBLOCK)
		{
			bytesRead = 0;
		}
		else
		{
			return SOCKET_ERROR;
		}
	}
	return bytesRead;
}