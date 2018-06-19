#include "AIAgentManager.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

AIAgentManager::AIAgentManager() :
	m_pTargetEntity(nullptr)
{
	// Private constructor
}

AIAgentManager* AIAgentManager::GetInstance()
{
	static AIAgentManager instace;
	return &instace;
}

void AIAgentManager::SetTargetEntity(const Entity* pEntity)
{		
	m_pTargetEntity = pEntity;
}

const Entity& AIAgentManager::GetTargetEntity() const
{
	return *m_pTargetEntity;
}

void AIAgentManager::Update(float deltaTime)
{
	// Update all agents
	for (AIAgent* agent : m_allActiveAgents)
	{
		agent->update(deltaTime);
	}
}

void AIAgentManager::addAgentConfig(const String& type,
	const String& configPath)
{
	m_agentConfigPathMap[type] = configPath;
}

void AIAgentManager::SpawnAgent(const String& type, const Vector2& position)
{
	if(Utils::ContainsKey(m_agentConfigPathMap, type))
	{
		AIAgent* agent = AIAgent::Create(
			m_agentConfigPathMap.at(type));
		m_allActiveAgents.push_back(agent);

		// Set agents camera mask same as world layer so that it is visible to camera
		agent->setCameraMask(m_worldLayer->getCameraMask());
		m_worldLayer->addChild(agent);
		agent->setPosition(position);

		// Actors spawn position is also his base position
		agent->SetBasePosition(position);
	}
	else
	{
		Utils::AssertWithStrFormat(false,
			"AIAgentManager: Trying to spawn agent of unknown type: %s",
			type);
	}	
}

bool AIAgentManager::Init(const String& pathToXML)
{	
	return XMLLoader::InitializeAIManagerUsingXMLFile(*this,
		pathToXML);
}

void AIAgentManager::SetWorldLayer(cocos2d::Node* pWorldLayer)
{
	m_worldLayer = pWorldLayer;
}

NS_LIGHTSOULS_END
