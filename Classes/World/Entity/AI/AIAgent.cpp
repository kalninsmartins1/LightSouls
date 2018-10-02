#include "AIAgent.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "World/Entity/Components/Attack/GenericAttackComponent.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/StateMachine/StateMachine.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "Events/OnCollisionBeginEventData.h"
#include "Utils/XML/XMLConsts.h"
#include "World/Entity/AI/StateMachine/States/AState.h"
#include "Scenes/GameScene.h"
#include "World/Entity/AI/AIAgentManager.h"

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
{

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

		GenericAttackComponent* attackComponent = dynamic_cast<GenericAttackComponent*>(getComponent(ATTACK_COMPONENT));
		const bool isAttackComponentFound = attackComponent != nullptr;
		CCASSERT(isAttackComponentFound, "AIAgent: AIAttackComponent not found !");
		m_attackComponent = attackComponent;

		// Register for physics events
		GameScene::GetPhysicsManager()->AddContactBeginListener(getName(),
			CC_CALLBACK_2(AIAgent::OnContactBegin, this));
		
		m_stateMachine.Start(GetAnimComponent());

		// init was successful only if the attack component are found
		isInitializedSuccessfully = isInitializedSuccessfully &&  isAttackComponentFound;
	}
	
	return isInitializedSuccessfully;
}

void AIAgent::Init(const XMLElement* element)
{	
	String startStateType;
	XMLLoader::ReadXMLAttribute(element, XML_AI_START_STATE, startStateType);
	EAIState startState = AState::GetStateFromString(startStateType);
	m_stateMachine.SetStartState(startState);

	CCASSERT(startState != EAIState::NONE, "AI start state is not set !");
	
	// Initialize states
	for (const XMLElement* stateXML = element->FirstChildElement(); stateXML != nullptr;
		stateXML = stateXML->NextSiblingElement())
	{
		String type = stateXML->Attribute(XML_TYPE_ATTR);
		EAIState state = AState::GetStateFromString(type);
		if (state != EAIState::NONE)
		{			
			m_stateMachine.AddAvailableState(state, stateXML);
		}
	}
}

bool AIAgent::OnContactBegin(const Vector2& contactPoint, const cocos2d::PhysicsBody* otherBody)
{
	cocos2d::Node* node = otherBody->getNode();
	if (node != nullptr)
	{
		m_stateMachine.DispatchEvent(PhysicsManager::GetEventOnCollisionBegin(),
			OnCollisionBeginEventData(GetId(), node->getName(), contactPoint));
	}

	return true;
}

void AIAgent::update(float deltaTime)
{
	Entity* targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	if (targetEntity != nullptr && targetEntity->GetCurrentHealth() > 0)
	{
		Entity::update(deltaTime);
		m_stateMachine.OnStep();
	}
	else
	{
		SetMoveDirection(Vector2::ZERO);
	}
}

void AIAgent::Reset()
{
	ResetMoveSpeed();
	ResetStamina();
	ResetHealth();
	ResetIsTakingDamage();
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

}

const String& AIAgent::GetEventAgentDestroyed()
{
	return s_eventAgentDestroyed;
}

const String& AIAgent::GetEventAgentHealthChanged()
{
	return s_eventAgentHealthChanged;
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

GenericAttackComponent* AIAgent::GetAttackComponent() const
{
	return m_attackComponent;
}

EntityType AIAgent::GetEntityType() const
{
	return EntityType::AIAGENT;
}

EAIState AIAgent::GetCurrentAIState() const
{
	return m_stateMachine.GetCurrentState();
}

NS_LIGHTSOULS_END
