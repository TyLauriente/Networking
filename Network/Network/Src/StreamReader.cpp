#include "Precompiled.h"
#include "StreamReader.h"

using namespace Network;

StreamReader::StreamReader(MemoryStream& memoryStream)
	: m_memoryStream{ memoryStream }
{

}

void StreamReader::Read(void* object, uint32_t size)
{
	memcpy(object, m_memoryStream.m_buffer + m_memoryStream.m_head, size);
	m_memoryStream.m_head += size;
}

void StreamReader::Read(std::string& str)
{
	size_t length;
	Read(&length, sizeof(size_t));
	str.resize(length);
	Read(&str[0], length);
}