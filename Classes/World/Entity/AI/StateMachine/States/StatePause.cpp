#include "StatePause.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "GameConsts.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

StatePause::StatePause(AIAgent& aiAgent)
	: m_agent(aiAgent)
	, m_curProgress(StateProgress::IN_PROGRESS)
	, m_pauseTime(0.0f)
{

}

void StatePause::OnEnter(AnimComponent* animComponent)
{
	animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
	m_curProgress = StateProgress::IN_PROGRESS;

	Utils::StartTimerWithCallback(&m_agent,
		CC_CALLBACK_0(StatePause::OnPauseExpired, this), m_pauseTime);
}

StateProgress StatePause::OnStep()
{
	return m_curProgress;
}

void StatePause::OnExit()
{

}

void StatePause::OnEventReceived(const String & receivedEvent, const AEventData & eventData)
{

}

void StatePause::OnPauseExpired()
{
	m_curProgress = StateProgress::DONE;
}

AIState StatePause::GetStateType() const
{
	return AIState::PAUSE;
}

void StatePause::SetPauseTime(float time)
{
	m_pauseTime = time;
}

NS_LIGHTSOULS_END