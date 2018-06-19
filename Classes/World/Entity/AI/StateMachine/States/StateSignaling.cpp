#include "StateSignaling.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "GameConsts.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

StateSignaling::StateSignaling(AIAgent& aiAgent)
	: m_agent(aiAgent)
	, m_animComponent(nullptr)
	, m_curProgress(StateProgress::IN_PROGRESS)
{

}

void StateSignaling::OnEnter(AnimComponent* animComponent)
{
	m_animComponent = animComponent;
	m_animComponent->PlayLoopingAnimation(ANIM_TYPE_SIGNAL);
	m_curProgress = StateProgress::IN_PROGRESS;

	Utils::StartTimerWithCallback(&m_agent,
		CC_CALLBACK_0(StateSignaling::OnFinishedSignaling, this), m_signalTime);
}

StateProgress StateSignaling::OnStep()
{
	return m_curProgress;
}

void StateSignaling::OnExit()
{
	m_animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
}

void StateSignaling::OnEventReceived(const String & receivedEvent, const AEventData & eventData)
{

}

void StateSignaling::OnFinishedSignaling()
{
	m_curProgress = StateProgress::DONE;
}

AIState StateSignaling::GetStateType() const
{
	return AIState::SIGNALING;
}

void StateSignaling::SetSignalingTime(float time)
{
	m_signalTime = time;
}

NS_LIGHTSOULS_END