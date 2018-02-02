#include "AIAgent.h"
#include "Utils/XML/XMLLoader.h"


AIAgent* AIAgent::create(const String& pathToXML)
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

AIAgent::AIAgent() :
	m_stateMachine(*this),
	m_basePosition(Vector2::ZERO),
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

void AIAgent::setBasePosition(const Vector2& position)
{
	m_basePosition = position;
}

void AIAgent::setAgentType(const String& type)
{
	m_agentType = type;
}

bool AIAgent::init(const String& pathToXML)
{	
	const bool bIsInitializedSuccessfully = 
		XMLLoader::initializeSpriteUsingXMLFile(*this, pathToXML);
	if(bIsInitializedSuccessfully)
	{
		m_stateMachine.start();
	}
	
	return bIsInitializedSuccessfully;
}

void AIAgent::update(float deltaTime)
{
	m_stateMachine.onStep();
}

float AIAgent::getPatrolPause() const
{
	return m_patrolPauseInSeconds;
}

float AIAgent::getActiveRadius() const
{
	return m_workingRadius;
}

float AIAgent::getAttackRadius() const
{
	return m_attackRadius;
}

const String& AIAgent::getType() const
{
	return m_agentType;
}

const Vector2& AIAgent::getBasePosition() const
{
	return m_basePosition;
}

void AIAgent::setPatrolPause(float pauseInSeconds)
{
	m_patrolPauseInSeconds = pauseInSeconds;
}

