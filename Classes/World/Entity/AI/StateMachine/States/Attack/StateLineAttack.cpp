#include "StateLineAttack.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Physics/PhysicsManager.h"
#include "World/Entity/Components/Attack/GenericAttackComponent.h"
#include "World/Entity/Entity.h"
#include "Events/OnCollisionBeginEventData.h"
#include "Utils/XML/XMLConsts.h"
#include "GameConsts.h"

NS_LIGHTSOULS_BEGIN

StateLineAttack::StateLineAttack(AIAgent& aiAgent)	
	: AState(aiAgent)	
	, m_curProgress(EStateProgress::NONE)
	, m_targetEntity(nullptr)
	, m_targetPosition(Vector2::ZERO)
	, m_attackComponent(nullptr)
	, m_moveSpeed(100.0f)
{

}

EAIState StateLineAttack::GetStateType() const
{
	return EAIState::LINE_ATTACK;
}

void StateLineAttack::OnEnter(AnimComponent * animComponent)
{		
	m_targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	m_targetPosition = m_targetEntity->getPosition();

	AIAgent& agent = GetAgent();	
	m_attackComponent = agent.GetAttackComponent();	

	if (m_attackComponent->IsReadyToAttack(m_targetPosition))
	{				
		m_curProgress = EStateProgress::IN_PROGRESS;				
		agent.SetCurrentMoveSpeed(m_moveSpeed); // Move with different speed in this state
	}
	else
	{
		m_curProgress = EStateProgress::FAILED;
	}	
}

EStateProgress StateLineAttack::OnStep()
{
	if (m_curProgress == EStateProgress::IN_PROGRESS)
	{
		AIAgent& agent = GetAgent();

		// Move to target
		Vector2 toTarget = m_targetPosition - agent.getPosition();
		agent.SetMoveDirection(toTarget.getNormalized());
		
		// Check if we are already there
		if (toTarget.length() <= (agent.GetCurrentMoveSpeed() / 10.0f))
		{
			m_curProgress = EStateProgress::DONE;
		}
	}
		
	return m_curProgress;
}

void StateLineAttack::OnExit()
{
	if(m_curProgress == EStateProgress::DONE)
	{
		m_attackComponent->Attack(GetAgent().getPosition() -
			m_targetEntity->getPosition());
	}
	AIAgent& agent = GetAgent();
	agent.SetMoveDirection(Vector2::ZERO);
	agent.ResetMoveSpeed();
}

void StateLineAttack::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{
	if (receivedEvent == PhysicsManager::GetEventOnCollisionBegin())
	{
		auto colData = static_cast<const OnCollisionBeginEventData&>(eventData);
		if (colData.GetCollidedWithName() == m_targetEntity->getName())
		{
			m_curProgress = EStateProgress::DONE;			
			m_targetEntity->TakeDamage(GetAgent());
		}
	}
	else if (receivedEvent == AIAgent::GetEventAgentHealthChanged())
	{
		m_curProgress = EStateProgress::FAILED;
	}
}

void StateLineAttack::LoadXMLData(const XMLElement* xmlElement)
{
	AState::LoadXMLData(xmlElement);
	m_moveSpeed = xmlElement->FloatAttribute(XML_MOVE_SPEED_ATTR);
}

NS_LIGHTSOULS_END