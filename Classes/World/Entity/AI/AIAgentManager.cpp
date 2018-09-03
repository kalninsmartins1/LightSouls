#include "AIAgentManager.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/Utils.h"
#include "SpawnPoint.h"

NS_LIGHTSOULS_BEGIN

AIAgentManager::AIAgentManager()
	: m_worldLayer(nullptr)
	, m_targetEntity(nullptr)
	, m_allActiveSpawnPoints()
	, m_agentConfigPathMap()
{
	// Private constructor
}

AIAgentManager* AIAgentManager::GetInstance()
{
	static AIAgentManager instace;
	return &instace;
}

void AIAgentManager::SetTargetEntity(Entity* pEntity)
{
	m_targetEntity = pEntity;
}

Entity* AIAgentManager::GetTargetEntity() const
{
	return m_targetEntity;
}

void AIAgentManager::Update(float deltaTime)
{
	if (m_targetEntity != nullptr)
	{
		for (auto spawnPoint : m_allActiveSpawnPoints)
		{
			spawnPoint->Update(deltaTime);
		}
	}	
}

void AIAgentManager::AddAgentConfig(const String& type,
	const String& configPath)
{
	m_agentConfigPathMap[type] = configPath;
}

void AIAgentManager::AddSpawnPoint(const SpawnPointConfig& config)
{
	auto spawnPoint = SpawnPoint::Create(config);
	m_worldLayer->addChild(spawnPoint);
	spawnPoint->SpawnAllAgents();
	m_allActiveSpawnPoints.push_back(spawnPoint);
}

void AIAgentManager::Cleanup()
{
	m_allActiveSpawnPoints.clear();
	m_agentConfigPathMap.clear();
	m_worldLayer = nullptr;
	m_targetEntity = nullptr;
}

void AIAgentManager::GetPathToAgentType(const String& type, String& outPath) const
{
	if(Utils::ContainsKey(m_agentConfigPathMap, type))
	{
		outPath = m_agentConfigPathMap.at(type);
	}
	else
	{
		outPath = "";
		Utils::AssertWithStrFormat(false,
			"AIAgentManager: Trying to spawn agent of unknown type: %s",
			type);
	}	
}

bool AIAgentManager::Init(cocos2d::Node* worldLayer, const String& pathToXML)
{
	bool isSuccessfullyInitialized = worldLayer != nullptr;
	if (isSuccessfullyInitialized)
	{
		m_worldLayer = worldLayer;
		isSuccessfullyInitialized = isSuccessfullyInitialized && XMLLoader::InitializeAIManagerUsingXMLFile(*this,
			pathToXML);
	}

	return isSuccessfullyInitialized;
}

NS_LIGHTSOULS_END
