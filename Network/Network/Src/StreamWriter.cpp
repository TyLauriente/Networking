#include "Precompiled.h"
#include "StreamWriter.h"

using namespace Network;

StreamWriter::StreamWriter(MemoryStream& memoryStream)
	: m_memoryStream{ memoryStream }
{

}

void StreamWriter::Write(const void* data, uint32_t size)
{
	const uint32_t newHead = m_memoryStream.m_head + size;
	if (newHead > m_memoryStream.m_capacity)
	{
		m_memoryStream.ReallocBuffer(std::max(m_memoryStream.m_capacity * 2, newHead));
	}

	memcpy(m_memoryStream.m_buffer + m_memoryStream.m_head, data, size);
	m_memoryStream.m_head += size;
}

void StreamWriter::Write(const std::string& str)
{
	const size_t length = str.length();
	Write(length);
	Write(str.c_str(), length);
}