#include "AIAgent.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "World/Entity/Components/Attack/AttackComponent.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/StateMachine/StateMachine.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "Events/OnCollisionBeginEventData.h"
#include "Utils/XML/XMLConsts.h"
#include "World/Entity/AI/StateMachine/States/AState.h"

NS_LIGHTSOULS_BEGIN

String AIAgent::s_eventAgentHealthChanged = "EVENT_AGENT_HEALTH_CHANGED";
String LightSouls::AIAgent::s_eventAgentDestroyed = "EVENT_AGENT_DESTROYED";

AIAgent* AIAgent::Create(const String& pathToXML)
{
	AIAgent* agent = new (std::nothrow) AIAgent();
	if (agent && agent->Init(pathToXML))
	{
		agent->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(agent);
	}

	return agent;
}

void AIAgent::DispatchOnGiveDamageEvent() const
{

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
	
	setLocalZOrder(AGENT_LAYER);

	if(isInitializedSuccessfully)
	{
		OnEntityInitialized();
		m_stateMachine.Start(GetAnimComponent());

		AttackComponent* attackComponent = dynamic_cast<AttackComponent*>(getComponent(ATTACK_COMPONENT));
		const bool isAttackComponentFound = attackComponent != nullptr;
		CCASSERT(isAttackComponentFound, "AIAgent: AIAttackComponent not found !");
		m_attackComponent = attackComponent;		

		// Register for physics events
		PhysicsManager::GetInstance()->AddContactBeginListener(getName(),
			CC_CALLBACK_1(AIAgent::OnContactBegin, this));

		// init was successful only if the attack component are found
		isInitializedSuccessfully = isInitializedSuccessfully &&  isAttackComponentFound;
	}
	
	return isInitializedSuccessfully;
}

void AIAgent::Init(const XMLElement* element)
{
	const float chaseRadius = element->FloatAttribute(XML_AI_CHASE_RADIUS_ATTR);
	const float chaseStopDistance = element->FloatAttribute(XML_AI_CHASE_STOP_DISTANCE);
	const float patrolRadius = element->FloatAttribute(XML_AI_PATROL_RADIUS_ATTR);
	const float patrolPause = element->FloatAttribute(XML_AI_PATROL_PAUSE_ATTR);
	
	String startStateType;
	XMLLoader::ReadXMLAttribute(element, XML_AI_START_STATE, startStateType);
	AIState startState = AState::GetStateFromString(startStateType);
	m_stateMachine.SetStartState(startState);

	CCASSERT(startState != AIState::NONE, "AI start state is not set !");
	CCASSERT(chaseRadius > 1, "AI chase radius is too small !");
	CCASSERT(chaseStopDistance > 0, "AI chase distance cant be negative !");
	CCASSERT(patrolRadius > 1, "AI patrol radius is too small !");

	SetChaseRadius(chaseRadius);
	SetChaseStopDistance(chaseStopDistance);
	SetPatrolRadius(patrolRadius);
	SetPatrolPause(patrolPause);
	
	// Initialize states
	for (const XMLElement* stateXML = element->FirstChildElement(); stateXML != nullptr;
		stateXML = stateXML->NextSiblingElement())
	{
		String type = stateXML->Attribute(XML_TYPE_ATTR);
		AIState state = AState::GetStateFromString(type);
		if (state != AIState::NONE)
		{
			String nextSuccessType;
			XMLLoader::ReadXMLAttribute(stateXML, XML_AI_NEXT_STATE_ON_SUCCESS, nextSuccessType);
			String nextFailureType;
			XMLLoader::ReadXMLAttribute(stateXML, XML_AI_NEXT_STATE_ON_FAILURE, nextFailureType);
			
			float timeRestriction = stateXML->FloatAttribute(XML_TIME_ATTR);
			AIState nextSuccessState = AState::GetStateFromString(nextSuccessType);
			AIState nextFailureState = AState::GetStateFromString(nextFailureType);

			m_stateMachine.AddAvailableState(state, nextSuccessState, nextFailureState, timeRestriction);
		}
	}
}

bool AIAgent::OnContactBegin(const cocos2d::PhysicsBody* otherBody)
{
	cocos2d::Node* node = otherBody->getNode();
	if (node != nullptr)
	{
		m_stateMachine.DispatchEvent(PhysicsManager::GetEventOnCollisionBegin(),
			OnCollisionBeginEventData(GetId(), node->getName()));
	}

	return true;
}

void AIAgent::update(float deltaTime)
{
	Entity::update(deltaTime);
	m_stateMachine.OnStep();
}

void AIAgent::Reset()
{
	ResetMoveSpeed();
	ResetStamina();
	ResetHealth();
	m_stateMachine.Reset();
	setPosition(GetBasePosition());
}

void AIAgent::DispatchOnStaminaChangedEvent() const
{

}

void AIAgent::DispatchOnHealthChangedEvent()
{
	m_stateMachine.DispatchEvent(s_eventAgentHealthChanged, AEventData(GetId()));
	if (GetHealth() <= 0)
	{
		getEventDispatcher()->dispatchCustomEvent(s_eventAgentDestroyed);
	}
}

AIAgent::~AIAgent()
{
	PhysicsManager::GetInstance()->RemoveContactBeginListener(getName());
}

const String& AIAgent::GetEventAgentDestroyed()
{
	return s_eventAgentDestroyed;
}

const String& AIAgent::GetEventAgentHealthChanged()
{
	return s_eventAgentHealthChanged;
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

EntityType AIAgent::GetEntityType() const
{
	return EntityType::AIAGENT;
}

void AIAgent::SetPatrolPause(float pauseInSeconds)
{
	m_patrolPauseInSeconds = pauseInSeconds;
}

NS_LIGHTSOULS_END
