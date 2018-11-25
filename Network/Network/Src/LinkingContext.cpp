#include "Precompiled.h"
#include "LinkingContext.h"

using namespace Network;


int LinkingContext::GetNetworkId(void* instance) const
{
	if (m_InstanceToNetworkIdMap.find(instance) != m_InstanceToNetworkIdMap.end())
	{
		return m_InstanceToNetworkIdMap.at(instance);
	}
	else
	{
		return sInvalidId;
	}
}

void* LinkingContext::GetInstance(int networkId) const
{
	if (m_networkIdToInstanceMap.find(networkId) != m_networkIdToInstanceMap.end())
	{
		return m_networkIdToInstanceMap.at(networkId);
	}
	else
	{
		return nullptr;
	}
}

int LinkingContext::Register(void* instance)
{
	int key = m_nextNetworkId;
	m_networkIdToInstanceMap[key] = instance;
	m_InstanceToNetworkIdMap[instance] = key;
	m_nextNetworkId++;
	return key;
}

bool LinkingContext::Register(void* instance, int networkId)
{
	if (m_networkIdToInstanceMap.find(networkId) != m_networkIdToInstanceMap.end())
	{
		m_networkIdToInstanceMap[networkId] = instance;
		m_InstanceToNetworkIdMap[instance] = networkId;
		return true;
	}
	return false;
}

void LinkingContext::Unregister(void* instance)
{
	int key = m_InstanceToNetworkIdMap[instance];
	m_InstanceToNetworkIdMap.erase(instance);
	m_networkIdToInstanceMap.erase(key);
}