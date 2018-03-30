#include "AIAgent.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"


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
		XMLLoader::InitializeEntityUsingXMLFile(*this, pathToXML);
	if(bIsInitializedSuccessfully)
	{
		AIAnimComponent* pAnimComponent = dynamic_cast<AIAnimComponent*>(getComponent(AI_ANIM_COMPONENT));
		CC_ASSERT(pAnimComponent != nullptr && "AIAgent: AIAnimComponent not found !");
			
		m_stateMachine.start(pAnimComponent);		
	}
	
	return bIsInitializedSuccessfully;
}

void AIAgent::update(float deltaTime)
{
	Entity::update(deltaTime);
	m_stateMachine.onStep();
}

float AIAgent::getPatrolPause() const
{
	return m_patrolPauseInSeconds;
}

float AIAgent::GetActiveRadius() const
{
	return m_workingRadius;
}

float AIAgent::GetAttackRadius() const
{
	return m_attackRadius;
}

const Entity::String& AIAgent::getType() const
{
	return m_agentType;
}

const Entity::Vector2& AIAgent::getBasePosition() const
{
	return m_basePosition;
}

void AIAgent::setPatrolPause(float pauseInSeconds)
{
	m_patrolPauseInSeconds = pauseInSeconds;
}

