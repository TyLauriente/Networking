#pragma once

#include "MemoryStream.h"

namespace Network
{
class StreamWriter
{
public:
	StreamWriter(MemoryStream& memoryStream);

	void Write(const void* data, uint32_t size);

	void Write(const std::string& str);

	template<class T>
	void Write(T data)
	{
		static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>,
			"Generic Write only supports primitive data types");
		Write(&data, sizeof(T));
	}

	template <class T>
	void Write(const std::vector<T>& data)
	{
		Write(data.size());
		for (const T& item : data)
			Write(item);
	}

private:
	MemoryStream& m_memoryStream;
};
}