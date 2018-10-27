#include "Precompiled.h"
#include "UDPSocket.h"

using namespace Network;

UDPSocket::UDPSocket()
	: m_socket(INVALID_SOCKET)
{

}

UDPSocket::~UDPSocket()
{
	Close();
}

bool UDPSocket::Open()
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		return false;
	}
	return true;
}

void UDPSocket::Close()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

bool UDPSocket::Bind(const SocketAddress& fromAddress)
{
	if (m_socket == INVALID_SOCKET && !Open())
	{
		return false;
	}

	int result = bind(m_socket, &fromAddress.m_sockAddr, fromAddress.GetSize());
	if (result == SOCKET_ERROR)
	{
		//WSAGetLastError() returns the last error message from winsock
		return false;
	}
	return true;
}

bool UDPSocket::SetBroadcast(bool broadcast)
{
	int result = setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(int));
	if (result == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

int UDPSocket::SendTo(const void* buffer, int len, const SocketAddress& toAddress)
{
	if (m_socket == INVALID_SOCKET && !Open())
	{
		return SOCKET_ERROR;
	}
	//if (m_socket == INVALID_SOCKET)
	//{
	//	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//}

	int bytesSent = sendto(m_socket, static_cast<const char*>(buffer),
		len, 0, &toAddress.m_sockAddr, toAddress.GetSize());
	if (bytesSent < 0)
	{
		return SOCKET_ERROR;
	}
	return bytesSent;
}
int UDPSocket::ReceiveFrom(void* buffer, int len, SocketAddress& fromAddress)
{
	int fromLength = fromAddress.GetSize();
	int bytesRead = recvfrom(m_socket, static_cast<char*>(buffer), len,
		0, &fromAddress.m_sockAddr, &fromLength);
	if (bytesRead >= 0)
	{
		return bytesRead;
	}
	else
	{
		int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK)
		{
			return 0;
		}
		else if (error == WSAECONNRESET)
		{
			return -WSAECONNRESET;
		}
		else
		{
			return -error;
		}
	}
}
