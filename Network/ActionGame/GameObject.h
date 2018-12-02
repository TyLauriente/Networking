#pragma once

#include <Network.h>
#include <XEngine.h>


class GameObject
{
public:
	ABSTRACT_CLASS_ID(GameObject, 'GOBJ')
	GameObject(bool sync);
	
	virtual void XInitialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	virtual void Serialize(Network::StreamWriter& writer) const = 0;
	virtual void Deserialize(Network::StreamReader& reader) = 0;

	void Kill()							{ m_destroyed = true;}
	void Clean()						{ m_dirty = false; }
	void Dirty()						{ m_dirty = true; }

	uint32_t GetNetworkId() const		{ return m_networkId; }
	bool IsActive() const				{ return !m_destroyed; }
	bool IsSync() const					{ return m_sync; }
	bool IsDirty() const				{ return m_dirty; }

private:
	friend class GameObjectManager;

	uint32_t m_networkId;
	bool m_destroyed;
	bool m_sync;
	bool m_dirty;
};

using GameObjectList = std::vector<GameObject*>;

