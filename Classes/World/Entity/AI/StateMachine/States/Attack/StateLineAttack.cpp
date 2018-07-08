#include "StateLineAttack.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Physics/PhysicsManager.h"
#include "World/Entity/Components/Attack/GenericAttackComponent.h"
#include "World/Entity/Entity.h"
#include "Events/OnCollisionBeginEventData.h"
#include "GameConsts.h"

NS_LIGHTSOULS_BEGIN

StateLineAttack::StateLineAttack(AIAgent& aiAgent)	
	: m_curProgress(EStateProgress::NONE)
	, m_agent(aiAgent)
	, m_targetEntity(nullptr)
	, m_targetPosition(Vector2::ZERO)
	, m_attackComponent(nullptr)
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
	m_attackComponent = m_agent.GetAttackComponent();
	m_curProgress = EStateProgress::IN_PROGRESS;
}

EStateProgress StateLineAttack::OnStep()
{

	// Move to target
	Vector2 toTarget = m_targetPosition - m_agent.getPosition();
	m_agent.SetMoveDirection(toTarget.getNormalized());

	// Check if we are already there
	if (toTarget.length() <= (m_agent.GetCurrentMoveSpeed()/10.0f))
	{
		m_curProgress = EStateProgress::DONE;
	}
	
	return m_curProgress;
}

void StateLineAttack::OnExit()
{
	m_agent.SetMoveDirection(Vector2::ZERO);
}

void StateLineAttack::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{
	if (receivedEvent == PhysicsManager::GetEventOnCollisionBegin())
	{
		auto colData = static_cast<const OnCollisionBeginEventData&>(eventData);
		if (colData.GetCollidedWithName() == m_targetEntity->getName())
		{
			m_curProgress = EStateProgress::DONE;
			m_attackComponent->Attack(m_agent.getPosition() -
				m_targetEntity->getPosition());
			m_targetEntity->TakeDamage(m_agent);
		}
	}
}

NS_LIGHTSOULS_END