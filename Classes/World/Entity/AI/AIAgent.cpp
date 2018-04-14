#include "AIAgent.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "World/Entity/Components/AttackComponent.h"

AIAgent* AIAgent::Create(const String& pathToXML)
{
	AIAgent* pAgent = new (std::nothrow) AIAgent();
	if (pAgent && pAgent->Init(pathToXML))
	{
		pAgent->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pAgent);
	}

	return pAgent;
}

AIAgent::AIAgent() 
	: m_stateMachine(*this)
	, m_basePosition(Vector2::ZERO)
	, m_chaseRadius(0)
	, m_patrolRadius(0)
	, m_patrolPauseInSeconds(0)
	, m_chaseStopDistance(0)
{
}

void AIAgent::SetPatrolRadius(float radius)
{
	m_patrolRadius = radius;
}

void AIAgent::SetChaseRadius(float radius)
{
	m_chaseRadius = radius;
}

void AIAgent::SetChaseStopDistance(float distance)
{
	m_chaseStopDistance = distance;
}

void AIAgent::setBasePosition(const Vector2& position)
{
	m_basePosition = position;
}

void AIAgent::SetAgentType(const String& type)
{
	m_agentType = type;
}

bool AIAgent::Init(const String& pathToXML)
{	
	bool isInitializedSuccessfully = 
		XMLLoader::InitializeEntityUsingXMLFile(*this, pathToXML);
	if(isInitializedSuccessfully)
	{
		AIAnimComponent* animComponent = dynamic_cast<AIAnimComponent*>(getComponent(AI_ANIM_COMPONENT));
		const bool isAnimComponentFound = animComponent != nullptr;
		CC_ASSERT( isAnimComponentFound && "AIAgent: AIAnimComponent not found !");			
		m_stateMachine.Start(animComponent);

		AttackComponent* attackComponent = dynamic_cast<AttackComponent*>(getComponent(ATTACK_COMPONENT));
		const bool isAttackComponentFound = attackComponent != nullptr;
		CC_ASSERT(isAttackComponentFound && "AIAgent: AIAttackComponent not found !");
		m_attackComponent = attackComponent;

		SetPhysicsBodyAnchor(Vector2(0, 0));

		// init was successful only if the attack and anim compoents are found
		isInitializedSuccessfully = isInitializedSuccessfully &&
			isAnimComponentFound &&  isAttackComponentFound;
	}
	
	return isInitializedSuccessfully;
}

void AIAgent::update(float deltaTime)
{
	Entity::update(deltaTime);
	m_stateMachine.OnStep();
}

float AIAgent::GetPatrolPause() const
{
	return m_patrolPauseInSeconds;
}

float AIAgent::GetPatrolRadius() const
{
	return m_patrolRadius;
}

float AIAgent::GetChaseRadius() const
{
	return m_chaseRadius;
}

float AIAgent::GetChaseStopDistance() const
{
	return m_chaseStopDistance;
}

float AIAgent::GetAttackRange() const
{
	return m_attackComponent->GetAttackRange();
}

const Entity::String& AIAgent::GetType() const
{
	return m_agentType;
}

const Entity::Vector2& AIAgent::GetBasePosition() const
{
	return m_basePosition;
}

AttackComponent* AIAgent::GetAttackComponent() const
{
	return m_attackComponent;
}

void AIAgent::SetPatrolPause(float pauseInSeconds)
{
	m_patrolPauseInSeconds = pauseInSeconds;
}

