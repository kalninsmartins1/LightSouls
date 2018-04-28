#include "StateChase.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Entity/CustomActions/ChaseAction.h"
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

	// Start chasing player
	const auto chase = ChaseAction::Create(m_targetEntity, m_agent);
	const auto callBack = cocos2d::CallFunc::create(
		CC_CALLBACK_0(StateChase::OnTargetReached, this));

	auto sequence = ActionSequnce::Create();
	sequence->AddAction(chase);
	sequence->AddAction(callBack);
	m_agent.runAction(sequence);

	// Play run animation
	animComponent->PlayLoopingAnimation(ANIM_TYPE_RUN);
}

StateProgress StateChase::OnStep()
{
	return m_curProgress;
}

void StateChase::OnExit()
{
	m_curProgress = StateProgress::NONE;
	m_agent.SetMoveDirection(Vector2::ZERO);
}

AIState StateChase::GetStateType()
{
	return AIState::CHASE;
}

void StateChase::OnTargetReached()
{
	m_curProgress = StateProgress::DONE;
}

NS_LIGHTSOULS_END