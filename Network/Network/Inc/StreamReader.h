#pragma once

#include "MemoryStream.h"

namespace Network
{
class StreamReader
{
public:
	StreamReader(MemoryStream& memoryStream);

	void Read(void* object, uint32_t size);

	void Read(std::string& str);

	template <class T>
	void Read(T& object)
	{
		static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>,
			"Generic Read only supports primitive data types");
		Read(&object, sizeof(T));
	}

	template <class T>
	void Read(std::vector<T>& object)
	{
		for (T& item : object)
		{
			Read(&object);
		}
	}

	uint32_t GetRemainingDataSize() const
	{
		return m_memoryStream.m_capacity - m_memoryStream.m_head;
	}

private:
	MemoryStream& m_memoryStream;
};

} // namespace Network
