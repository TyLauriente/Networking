#include "GameObjectManager.h"

namespace
{
	GameObjectManager* sInstance = nullptr;
}

void GameObjectManager::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "[GameObjectManager] Manager already initialized");
	sInstance = new GameObjectManager();
}

void GameObjectManager::StaticTerminate()
{
	XASSERT(sInstance != nullptr, "[GameObjectManager] Manager already terminated");
	delete sInstance;
	sInstance = nullptr;
}

GameObjectManager* GameObjectManager::Get()
{
	XASSERT(sInstance != nullptr, "[GameObjectManager] Manager not initialized");
	return sInstance;
}

GameObjectManager::GameObjectManager()
{

}


GameObjectManager::~GameObjectManager()
{
	for (size_t i = 0; i < m_gameObjects.size(); ++i)
	{
		delete m_gameObjects[i];
		m_gameObjects[i] = nullptr;
	}
	m_gameObjects.clear();
}


Network::ObjectFactory& GameObjectManager::GetFactory()
{
	return m_objectFactory;
}

GameObject* GameObjectManager::CreateGameObject(uint32_t type)
{
	GameObject* newObject = (GameObject*)m_objectFactory.CreateInstance(type);
	m_gameObjects.push_back(newObject);

	newObject->m_networkId = m_linkingContext.Register(newObject);
	return newObject;
}

GameObject* GameObjectManager::CreateGameObject(Network::StreamReader& reader)
{
	uint32_t type = 0;
	uint32_t networkId = 0;
	reader.Read(type);
	reader.Read(networkId);

	GameObject* newObject = (GameObject*)m_objectFactory.CreateInstance(type);
	newObject->XInitialize();
	m_gameObjects.push_back(newObject);

	m_linkingContext.Register(newObject, networkId);
	newObject->Deserialize(reader);
	return newObject;
}

//TODO: GetId does not work right now
void GameObjectManager::DestoryGameObject(uint32_t id)
{
	auto gameObject = FindGameObject(id);
	if (gameObject)
	{
		gameObject->Kill();
	}
}


GameObject* GameObjectManager::FindGameObject(uint32_t networkId)
{
	auto iter = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
		[networkId](GameObject* obj) { return obj->GetNetworkId() == networkId; });
	if (iter != m_gameObjects.end())
	{
		return (*iter);
	}
}

void GameObjectManager::Serialize(Network::StreamWriter& writer, bool dirtyOnly) const
{
	GameObjectList gameObjectsToSync;
	gameObjectsToSync.reserve(m_gameObjects.size());

	// Count objects that need to be synced
	for (auto& gameObject : m_gameObjects)
	{
		if (gameObject->IsSync() && (!dirtyOnly || gameObject->IsDirty()))
		{
			gameObjectsToSync.emplace_back(gameObject);
		}
	}

	// Serialize object states
	writer.Write(gameObjectsToSync.size());
	for (auto& gameObject : gameObjectsToSync)
	{
		writer.Write(gameObject->GetClassId());
		writer.Write(gameObject->GetNetworkId());
		gameObject->Serialize(writer);
		gameObject->Clean();
	}
}

void GameObjectManager::Deserialize(Network::StreamReader& reader)
{
	XASSERT(m_gameObjects.empty(), "[GameObjectManager] Manager not empty!");

	size_t count = 0;
	reader.Read(count);
	for (size_t i = 0; i < count; ++i)
	{
		CreateGameObject(reader);
	}
}

void GameObjectManager::Update(float deltaTime)
{
	for (auto gameObject : m_gameObjects)
	{
		if (gameObject->IsActive())
		{
			gameObject->Update(deltaTime);
		}
	}
	for (size_t i = 0; i < m_gameObjects.size();)
	{
		GameObject* gameObject = m_gameObjects[i];
		if (!gameObject->IsActive())
		{
			m_linkingContext.Unregister(gameObject);
			delete gameObject;
			gameObject = nullptr;
			m_gameObjects[i] = m_gameObjects.back();
			m_gameObjects.pop_back();
		}
		else
		{
			++i;
		}
	}
}

void GameObjectManager::Update(Network::StreamReader& reader)
{
	size_t count = 0;
	reader.Read(count);

	for (size_t i = 0; i < count; ++i)
	{
		uint32_t type = 0;
		uint32_t networkId = 0;
		reader.Read(type);
		reader.Read(networkId);

		GameObject* newObject = (GameObject*)m_linkingContext.GetInstance(networkId);
		XASSERT(newObject != nullptr, "[GameObjectManager] Object %d not found", networkId);
		newObject->Deserialize(reader);
	}
}

void GameObjectManager::Draw()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->Draw();
	}
}