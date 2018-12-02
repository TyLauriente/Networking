#include"Precompiled.h"
#include"MemoryStream.h"

using namespace Network;

MemoryStream::MemoryStream(uint32_t capacity)
{
	ReallocBuffer(capacity);
}

MemoryStream::MemoryStream(const uint8_t* data, uint32_t size, bool takeOwnereShip)
	: m_capacity{ size }
{
	if (takeOwnereShip)
	{
		m_buffer = const_cast<uint8_t*>(data);
	}
	else
	{
		m_buffer = static_cast<uint8_t*>(std::malloc(size));
		memcpy(m_buffer, data, size);
	}
}

MemoryStream::~MemoryStream()
{
	std::free(m_buffer);
}

void MemoryStream::ReallocBuffer(uint32_t newCapacity)
{
	m_buffer = static_cast<uint8_t*>(std::realloc(m_buffer, newCapacity));
	m_capacity = newCapacity;
}