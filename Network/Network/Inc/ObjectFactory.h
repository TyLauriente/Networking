#pragma once

#include <Network.h>
#include "Common.h"

#define ABSTRACT_CLASS_ID(ClassType, FourCC)\
	static const uint32_t ClassId = FourCC;\
	virtual uint32_t GetClassId() const { return ClassId; }

#define CLASS_ID(ClassType, FourCC)\
	static void* CreateInstance() { return new ClassType(); }\
	static const uint32_t ClassId = FourCC;\
	virtual uint32_t GetClassId() const { return ClassId; }

namespace Network
{
class ObjectFactory
{
public:
	 template <class T>
	 void Register()
	 {
		 m_creators[T::ClassId] = T::CreateInstance;
	 }

	 void* CreateInstance(uint32_t classId)
	 {
		 return m_creators[classId]();
	 }

private:
	std::unordered_map<uint32_t, std::function<void*()>> m_creators;
};
}