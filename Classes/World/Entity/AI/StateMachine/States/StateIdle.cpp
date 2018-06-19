#include "StateIdle.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Entity/AI/AIAgent.h"
#include "GameConsts.h"

NS_LIGHTSOULS_BEGIN

StateIdle::StateIdle(AIAgent& aiAgent)
	: m_curProgress(StateProgress::IN_PROGRESS)	
{

}

AIState StateIdle::GetStateType() const
{
	return AIState::IDLE;
}

void StateIdle::OnEnter(AnimComponent * animComponent)
{
	m_curProgress = StateProgress::IN_PROGRESS;
	animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
}

StateProgress StateIdle::OnStep()
{
	return m_curProgress;
}

void StateIdle::OnExit()
{

}

void StateIdle::OnEventReceived(const String& receivedEvent, const AEventData & eventData)
{
	if (receivedEvent == AIAgent::GetEventAgentHealthChanged())
	{
		m_curProgress = StateProgress::DONE;
	}
}

NS_LIGHTSOULS_END