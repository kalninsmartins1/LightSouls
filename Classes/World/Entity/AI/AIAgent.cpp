#include "AIAgent.h"
#include "StateMachine/StateMachine.h"
#include "Utils/XML/XMLLoader.h"

using namespace cocos2d;


AIAgent* AIAgent::create(const std::string& pathToXML)
{
	AIAgent* pAgent = new (std::nothrow) AIAgent();
	if (pAgent && pAgent->init(pathToXML))
	{
		pAgent->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pAgent);
	}

	return pAgent;
}

AIAgent* AIAgent::create(AIAgent& agent)
{
	AIAgent* pAgent = new (std::nothrow) AIAgent();
	if (pAgent && pAgent->init(agent))
	{
		pAgent->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pAgent);
	}

	return pAgent;
}

AIAgent::AIAgent() :
	m_basePosition(Vec2::ZERO),
	m_workingRadius(0),
	m_attackRadius(0),
	m_patrolPauseInSeconds(0)
{
}

void AIAgent::setWorkingRadius(float radius)
{
	m_workingRadius = radius;
}

void AIAgent::setAttackRadius(float radius)
{
	m_attackRadius = radius;
}

void AIAgent::setBasePosition(const Vec2& position)
{
	m_basePosition = position;
}

void AIAgent::setAgentType(const std::string& type)
{
	m_agentType = type;
}

bool AIAgent::init(const std::string& pathToXML)
{	
	return XMLLoader::initializeSpriteUsingXMLFile(*this, pathToXML);
}

bool AIAgent::init(AIAgent& agent)
{	
	setAttackRadius(agent.m_attackRadius);
	setWorkingRadius(agent.m_workingRadius);
	setPatrolPause(agent.m_patrolPauseInSeconds);
	setBasePosition(agent.m_basePosition);
	setAgentType(agent.m_agentType);
	setBaseMoveSpeed(agent.m_baseMoveSpeed);
	initWithTexture(agent.getTexture());	
	
	// Start out moving at regular speed
	m_moveSpeed = m_baseMoveSpeed;
	m_pStateMachine = std::make_unique<StateMachine>(*this);

	return true;
}

void AIAgent::update(float deltaTime)
{
	m_pStateMachine->update();
}

float AIAgent::getPatrolPause() const
{
	return m_patrolPauseInSeconds;
}

float AIAgent::getActiveRadius() const
{
	return m_workingRadius;
}

const std::string& AIAgent::getType() const
{
	return m_agentType;
}

const Vec2& AIAgent::getBasePosition() const
{
	return m_basePosition;
}

void AIAgent::setPatrolPause(float pauseInSeconds)
{
	m_patrolPauseInSeconds = pauseInSeconds;
}

