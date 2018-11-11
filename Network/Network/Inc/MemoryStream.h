#pragma once

#include "SocketAddress.h"

namespace Network
{
class MemoryStream
{
public:
	MemoryStream() = default;
	MemoryStream(uint32_t capacity);
	MemoryStream(const uint8_t* data, uint32_t size, bool takeOwnereShip = false);
	~MemoryStream();

	void Reset()					{ m_head = 0; }

	const uint8_t* GetData() const	{ return m_buffer; }
	uint32_t GetHead() const		{ return m_head; }
	uint32_t Getcapacity() const	{ return m_capacity; }

private:
	MemoryStream(const MemoryStream&) = delete;
	MemoryStream& operator=(const MemoryStream&) = delete;

	friend class StreamReader;
	friend class StreamWriter;

	void ReallocBuffer(uint32_t newCapacity);

	uint8_t* m_buffer{ nullptr };
	uint32_t m_head{ 0 };
	uint32_t m_capacity{ 0 };
};

} // namespace Network