#pragma once

namespace Network
{
class SocketAddress
{
public:
	SocketAddress();
	SocketAddress(const char* host, uint16_t port);
	SocketAddress(uint16_t port);

	std::string GetHost() const;

private:
	friend class TCPSocket;
	friend class UDPSocket;

	sockaddr_in* GetAsSockAddrIn()				{ return reinterpret_cast<sockaddr_in*>(&m_sockAddr); }
	const sockaddr_in* GetAsSockAddrIn() const	{ return reinterpret_cast<const sockaddr_in*>(&m_sockAddr); }

	uint32_t& GetIP4()							{ return *reinterpret_cast<uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }
	const uint32_t& GetIP4() const				{ return *reinterpret_cast<const uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }

	uint16_t& GetPort()							{ return *reinterpret_cast<uint16_t*>(&GetAsSockAddrIn()->sin_port); }
	const uint16_t& GetPort() const				{ return *reinterpret_cast<const uint16_t*>(&GetAsSockAddrIn()->sin_port); }

	size_t GetSize() const						{ return sizeof(sockaddr); }

	sockaddr m_sockAddr;
};
}