#include "AIAgentManager.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/Utils.h"

using namespace cocos2d;


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

void AIAgentManager::addAgentConfig(const std::string& type,
	const std::string& configPath)
{
	m_agentConfigPathMap[type] = configPath;
}

void AIAgentManager::SpawnAgent(const std::string& type, const Vec2& position)
{
	if(Utils::ContainsKey(m_agentConfigPathMap, type))
	{
		AIAgent* pAgent = AIAgent::create(
			m_agentConfigPathMap.at(type));
		m_allActiveAgents.push_back(pAgent);

		// Set agents camera mask same as world layer so that it is visible to camera
		pAgent->setCameraMask(m_pWorldLayer->getCameraMask());
		m_pWorldLayer->addChild(pAgent);
		pAgent->setPosition(position);

		// Actors spawn position is also his base position
		pAgent->setBasePosition(position);
	}
	else
	{
		Utils::AssertWithStrFormat(false,
			"AIAgentManager: Trying to spawn agent of unknown type: %s",
			type);
	}	
}

bool AIAgentManager::Init(const std::string& pathToXML)
{	
	return XMLLoader::InitializeAIManagerUsingXMLFile(*this,
		pathToXML);
}

void AIAgentManager::SetWorldLayer(Node* pWorldLayer)
{
	m_pWorldLayer = pWorldLayer;
}
