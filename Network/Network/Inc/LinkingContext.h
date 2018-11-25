#pragma once

#include <Network.h>

namespace Network
{
class LinkingContext
{
public:
	static const int sInvalidId = -1;
public:
	LinkingContext() = default;

	int GetNetworkId(void* instance) const;
	void* GetInstance(int networkId) const;

	// Register an instance to the next network id
	int Register(void* instance);

	// Register an instance to a specific network id
	bool Register(void* instance, int networkId);

	// Unregister an instance
	void Unregister(void* instance);
	
private:
	std::unordered_map<int, void*> m_networkIdToInstanceMap;
	std::unordered_map<void*, int> m_InstanceToNetworkIdMap;
	int m_nextNetworkId;

};

} // Namespace Network