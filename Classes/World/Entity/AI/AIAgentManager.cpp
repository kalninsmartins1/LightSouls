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

void AIAgentManager::addAgent(const std::string& type, AIAgent* pAgent)
{
	m_agentPrefabMap.insert(type, pAgent);
}

void AIAgentManager::spawnAgent(const std::string& type, const Vec2& position)
{
	if(Utils::containsKey(m_agentPrefabMap, type))
	{
		AIAgent* pAgent = AIAgent::create(*m_agentPrefabMap.at(type));
		m_allActiveAgents.push_back(pAgent);
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
