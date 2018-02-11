#include "StateChase.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Entity/CustomActions/ChaseAction.h"

StateChase::StateChase(AIAgent& agent) :
	m_curProgress(StateProgress::NONE),
	m_targetEntity(AIAgentManager::getInstance()->getTargetEntity()),
	m_agent(agent)	
{
}

void StateChase::onEnter(AIAnimComponent* pAnimComponent)
{
	m_curProgress = StateProgress::IN_PROGRESS;

	// Start chasing player
	const auto pChase = ChaseAction::create(m_targetEntity, m_agent);
	const auto pCallBack = cocos2d::CallFunc::create(
		CC_CALLBACK_0(StateChase::onTargetReached, this));
	m_agent.runAction(cocos2d::Sequence::create(
		pChase,
		pCallBack,
		nullptr));

	// Play run animation
	pAnimComponent->playRunAnimation();
}

StateProgress StateChase::onStep()
{
	return m_curProgress;
}

void StateChase::onExit()
{
	m_curProgress = StateProgress::NONE;
}

AIState StateChase::getStateType()
{
	return AIState::CHASE;
}

void StateChase::onTargetReached()
{
	m_curProgress = StateProgress::DONE;
}
