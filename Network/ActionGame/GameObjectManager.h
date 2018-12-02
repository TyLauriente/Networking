#pragma once

#include  "GameObject.h"

class GameObjectManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static GameObjectManager* Get();
public:

	GameObjectManager();
	~GameObjectManager();

	Network::ObjectFactory& GetFactory();

	GameObject* CreateGameObject(uint32_t type);
	GameObject* CreateGameObject(Network::StreamReader& reader);
	void DestoryGameObject(uint32_t networkId);

	GameObject* FindGameObject(uint32_t networkId);

	void Serialize(Network::StreamWriter& writer, bool dirtyOnly) const;
	void Deserialize(Network::StreamReader& reader);

	void Update(float deltaTime);
	void Update(Network::StreamReader& reader);
	void Draw();

private:
	Network::ObjectFactory m_objectFactory;
	Network::LinkingContext m_linkingContext;
	GameObjectList m_gameObjects;
};

