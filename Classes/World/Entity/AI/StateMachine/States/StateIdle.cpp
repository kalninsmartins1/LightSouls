#include "StateIdle.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Entity/AI/AIAgent.h"
#include "GameConsts.h"



StateIdle::StateIdle(AIAgent& aiAgent)
	: AState(aiAgent)
	, m_curProgress(EStateProgress::IN_PROGRESS)
{

}

EAIState StateIdle::GetStateType() const
{
	return EAIState::IDLE;
}

void StateIdle::OnEnter(AnimComponent& animComponent)
{
	m_curProgress = EStateProgress::IN_PROGRESS;
	animComponent.PlayLoopingAnimation(GameConsts::ANIM_TYPE_IDLE);
}

EStateProgress StateIdle::OnStep()
{
	return m_curProgress;
}

void StateIdle::OnExit()
{

}

void StateIdle::OnEventReceived(const String& receivedEvent, const BaseEventData & eventData)
{
	if (receivedEvent == AIAgent::GetEventOnHealthChanged())
	{
		m_curProgress = EStateProgress::DONE;
	}
}

