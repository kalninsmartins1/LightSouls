#include "AIAgentManager.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/Utils.h"

using namespace cocos2d;


AIAgentManager::AIAgentManager() :
	m_pTargetEntity(nullptr)
{
	// Private constructor
}

AIAgentManager* AIAgentManager::getInstance()
{
	static AIAgentManager instace;
	return &instace;
}

void AIAgentManager::setTargetEntity(const Entity* pEntity)
{		
	m_pTargetEntity = pEntity;
}

const Entity& AIAgentManager::getTargetEntity() const
{
	return *m_pTargetEntity;
}

void AIAgentManager::update(float deltaTime)
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

void AIAgentManager::spawnAgent(const std::string& type, const Vec2& position)
{
	if(Utils::containsKey(m_agentConfigPathMap, type))
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
		Utils::assertWithStrFormat(false,
			"AIAgentManager: Trying to spawn agent of unknown type: %s",
			type);
	}	
}

bool AIAgentManager::init(const std::string& pathToXML)
{	
	return XMLLoader::initializeAIManagerUsingXMLFile(*this,
		pathToXML);
}

void AIAgentManager::setWorldLayer(Node* pWorldLayer)
{
	m_pWorldLayer = pWorldLayer;
}
