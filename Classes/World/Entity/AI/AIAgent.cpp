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
#include "World/Entity/CustomActions/AI/AIAvoidTargetAction.h"

String AIAgent::s_eventAgentHealthChanged = "EVENT_AGENT_HEALTH_CHANGED";
String AIAgent::s_eventAgentDamageTaken = "EVENT_AGENT_DAMAGE_TAKEN";
String AIAgent::s_eventAgentDestroyed = "EVENT_AGENT_DESTROYED";

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

		GenericAttackComponent* attackComponent = dynamic_cast<GenericAttackComponent*>(getComponent(GameConsts::ATTACK_COMPONENT));
		const bool isAttackComponentFound = attackComponent != nullptr;
		CCASSERT(isAttackComponentFound, "AIAgent: AIAttackComponent not found !");
		m_attackComponent = attackComponent;
		m_avoidAction = dynamic_cast<AIAvoidTargetAction*>(getActionByTag(GameConsts::ACTION_AI_TASK_AVOID));

		// Register for physics events
		GameScene::GetPhysicsManager()->AddContactBeginListener(getName(),
			CC_CALLBACK_2(AIAgent::OnContactBegin, this));
		
		AnimComponent* animComp = GetAnimComponent();
		if (animComp != nullptr)
		{
			m_stateMachine.Start(*animComp);
		}
		else
		{
			CCASSERT(false, "AIAgen::Init cant start StateMachine no AnimComponent found !");
		}

		// init was successful only if the attack component are found
		isInitializedSuccessfully = isInitializedSuccessfully &&  isAttackComponentFound;
	}
	
	return isInitializedSuccessfully;
}

void AIAgent::Init(const XMLElement* element)
{	
	String startStateType;
	XMLLoader::ReadXMLAttribute(element, XMLConsts::AI_START_STATE, startStateType);
	EAIState startState = AState::GetStateFromString(startStateType);
	m_stateMachine.SetStartState(startState);

	CCASSERT(startState != EAIState::NONE, "AI start state is not set !");
	
	// Initialize states
	for (const XMLElement* stateXML = element->FirstChildElement(); stateXML != nullptr;
		stateXML = stateXML->NextSiblingElement())
	{
		if (stateXML != nullptr)
		{
			String type = stateXML->Attribute(XMLConsts::TYPE_ATTR);
			EAIState state = AState::GetStateFromString(type);
			if (state != EAIState::NONE && stateXML != nullptr)
			{
				m_stateMachine.AddAvailableState(state, *stateXML);
			}
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

void AIAgent::Update(float deltaTime)
{
	Entity* targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	if (targetEntity != nullptr && targetEntity->GetCurrentHealth() > 0)
	{
		Entity::Update(deltaTime);
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

void AIAgent::DispatchOnHealthReduceEvent()
{
	m_stateMachine.DispatchEvent(s_eventAgentHealthChanged, AEventData(GetId()));
	m_stateMachine.DispatchEvent(s_eventAgentDamageTaken, AEventData(GetId()));

	if (GetHealth() <= 0)
	{
		getEventDispatcher()->dispatchCustomEvent(s_eventAgentDestroyed);
	}
}

AIAgent::~AIAgent()
{

}

const String& AIAgent::GetEventOnDestroyed()
{
	return s_eventAgentDestroyed;
}

const String& AIAgent::GetEventOnHealthChanged()
{
	return s_eventAgentHealthChanged;
}

const String& AIAgent::GetEventOnDamageTaken()
{
	return s_eventAgentDamageTaken;
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

bool AIAgent::IsBackgroundTaskReady() const
{	
	return m_avoidAction == nullptr || !m_avoidAction->IsAvoiding();
}


