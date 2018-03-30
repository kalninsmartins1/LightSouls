#include "StateChase.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Entity/CustomActions/ChaseAction.h"

StateChase::StateChase(AIAgent& agent) :
	m_curProgress(StateProgress::NONE),
	m_targetEntity(AIAgentManager::GetInstance()->GetTargetEntity()),
	m_agent(agent)	
{
}

void StateChase::OnEnter(AIAnimComponent* animComponent)
{
	m_curProgress = StateProgress::IN_PROGRESS;

	// Start chasing player
	const auto chase = ChaseAction::create(m_targetEntity, m_agent);
	const auto callBack = cocos2d::CallFunc::create(
		CC_CALLBACK_0(StateChase::OnTargetReached, this));
	m_agent.runAction(cocos2d::Sequence::create(
		chase,
		callBack,
		nullptr));

	// Play run animation
	animComponent->PlayRunAnimation();
}

StateProgress StateChase::OnStep()
{
	return m_curProgress;
}

void StateChase::OnExit()
{
	m_curProgress = StateProgress::NONE;
}

AIState StateChase::GetStateType()
{
	return AIState::CHASE;
}

void StateChase::OnTargetReached()
{
	m_curProgress = StateProgress::DONE;
}
