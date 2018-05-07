#include "StateChase.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Entity/CustomActions/ActionSequence.h"
#include "World/Entity/Components/AnimComponent.h"
#include "GameConsts.h"

NS_LIGHTSOULS_BEGIN

StateChase::StateChase(AIAgent& agent) :
	m_curProgress(StateProgress::NONE),
	m_targetEntity(AIAgentManager::GetInstance()->GetTargetEntity()),
	m_agent(agent)	
{
}

void StateChase::OnEnter(AnimComponent* animComponent)
{
	m_curProgress = StateProgress::IN_PROGRESS;
	m_animComponent = animComponent;
	m_animComponent->PlayLoopingAnimation(ANIM_TYPE_RUN);
}

StateProgress StateChase::OnStep()
{
	// Move agent towards target location
	const Vector2& currentPosition = m_agent.getPosition();
	Vector2 toTarget = m_targetEntity.getPosition() - currentPosition;
	const Vector2& toTargetNormalized = toTarget.getNormalized();

	// Update agent move direction
	m_agent.SetMoveDirection(toTargetNormalized);
	float distanceToTarget = toTarget.length();

	if (distanceToTarget <= m_agent.GetChaseStopDistance() ||	// Target has been caught
		distanceToTarget >= m_agent.GetChaseRadius()) // Target run off
	{
		m_curProgress = StateProgress::DONE;
	}
	return m_curProgress;
}

void StateChase::OnExit()
{
	m_curProgress = StateProgress::NONE;
	m_agent.SetMoveDirection(Vector2::ZERO);
	m_animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
}

void StateChase::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{
	// ...
}

AIState StateChase::GetStateType()
{
	return AIState::CHASE;
}

NS_LIGHTSOULS_END