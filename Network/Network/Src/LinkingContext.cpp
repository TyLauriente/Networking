#include "Precompiled.h"
#include "LinkingContext.h"

using namespace Network;


int LinkingContext::GetNetworkId(void* instance) const
{
	return 0;
}

void* LinkingContext::GetInstance(int networkId) const
{
	return nullptr;
}

int LinkingContext::Register(void* instance)
{
	return 0;
}

void LinkingContext::Register(void* instance, int networkId)
{

}

void LinkingContext::Unregister(void* instance)
{

}