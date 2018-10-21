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

}

void TCPSocket::Close()
{

}

bool TCPSocket::Connect(const SocketAddress& address)
{

}

bool TCPSocket::Bind(const SocketAddress& addrerss)
{

}

bool TCPSocket::Listen(int backLog = 10)
{

}

TCPSocket* Accept(SocketAddress& fromAddress)
{

}

int TCPSocket::Send(const void* buffer, int len)
{

}

int TCPSocket::Receive(void* buffer, int len)
{

}