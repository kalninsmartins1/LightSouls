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

#if LIGHTSOULS_DEBUG_AI
	CCLOG("StateChase: OnEnter!");
#endif
}

StateProgress StateChase::OnStep()
{
	if (!m_agent.IsProcessing())
	{
		// Move agent towards target location
		const Vector2& currentPosition = m_agent.getPosition();
		Vector2 toTarget = m_targetEntity.getPosition() - currentPosition;
		const Vector2& toTargetNormalized = toTarget.getNormalized();

		// Update agent move direction
		m_agent.SetMoveDirection(toTargetNormalized);
		float distanceToTarget = toTarget.length();

		// Play run animation if not playing, might be that on enter entity was processing
		if (!m_animComponent->IsCurrrentlyPlayingAnimation(ANIM_TYPE_RUN))
		{
			m_animComponent->PlayLoopingAnimation(ANIM_TYPE_RUN);
		}

		if (distanceToTarget <= m_agent.GetChaseStopDistance() ||	// Target has been caught
			distanceToTarget >= m_agent.GetChaseRadius())			// Target run off 
		{
			m_curProgress = StateProgress::DONE;
		}
	}

	return m_curProgress;
}

void StateChase::OnExit()
{
	m_curProgress = StateProgress::NONE;
	m_agent.SetMoveDirection(Vector2::ZERO);

	if (!m_agent.IsProcessing())
	{
		m_animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
	}

#if LIGHTSOULS_DEBUG_AI
	CCLOG("StateChase: OnExit!");
#endif
}

void StateChase::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{
	// ...
}

AIState StateChase::GetStateType() const
{
	return AIState::CHASE;
}

NS_LIGHTSOULS_END