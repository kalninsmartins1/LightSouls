#include "AIAgent.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "World/Entity/Components/AttackComponent.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/StateMachine/StateMachine.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "Events/OnCollisionBeginEventData.h"

NS_LIGHTSOULS_BEGIN

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

void AIAgent::SetBasePosition(const Vector2& position)
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
		OnEntityInitialized();

		AnimComponent* animComponent = dynamic_cast<AnimComponent*>(getComponent(ANIM_COMPONENT));
		const bool isAnimComponentFound = animComponent != nullptr;
		CC_ASSERT( isAnimComponentFound && "AIAgent: AIAnimComponent not found !");			
		m_stateMachine.Start(animComponent);

		AttackComponent* attackComponent = dynamic_cast<AttackComponent*>(getComponent(ATTACK_COMPONENT));
		const bool isAttackComponentFound = attackComponent != nullptr;
		CC_ASSERT(isAttackComponentFound && "AIAgent: AIAttackComponent not found !");
		m_attackComponent = attackComponent;

		SetPhysicsBodyAnchor(Vector2(0.0f, 0.0f));

		// Register for physics events
		PhysicsManager::GetInstance()->AddContactBeginListener(getName(),
			CC_CALLBACK_1(AIAgent::OnContactBegin, this));
		PhysicsManager::GetInstance()->AddContactEndListener(getName(),
			CC_CALLBACK_1(AIAgent::OnContactBegin, this));

		// init was successful only if the attack and anim compoents are found
		isInitializedSuccessfully = isInitializedSuccessfully &&
			isAnimComponentFound &&  isAttackComponentFound;
	}
	
	return isInitializedSuccessfully;
}

void AIAgent::OnContactBegin(const cocos2d::PhysicsBody* otherBody)
{
	cocos2d::Node* node = otherBody->getNode();
	if (node != nullptr)
	{
		m_stateMachine.DispatchEvent(PhysicsManager::GetEventOnCollisionBegin(),
			OnCollisionBeginEventData(GetId(), node->getName()));
	}	
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

float AIAgent::GetStoppingDistance() const
{
	return Utils::SafeDevide(GetCurrentMoveSpeed(), GetPhysicsBodyForceScale());
}

const String& AIAgent::GetType() const
{
	return m_agentType;
}

const Vector2& AIAgent::GetBasePosition() const
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

NS_LIGHTSOULS_END
