#include "StateAttack.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "World/Entity/Components/Attack/GenericAttackComponent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "Utils/AnimationUtils.h"
#include "Utils/Utils.h"



StateAttack::StateAttack(AIAgent& agent)
	: AState(agent)
	, m_curProgress(EStateProgress::NONE)
	, m_targetEntity(nullptr)
	, m_attackComponent(nullptr)
	, m_animComponent(nullptr)
	, m_isAnimFinished(true)
{

}

void StateAttack::OnEnter(AnimComponent& animComponent)
{
	m_targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	m_animComponent = &animComponent;
	m_attackComponent = GetAgent().GetAttackComponent();
	m_curProgress = EStateProgress::IN_PROGRESS;
}

EStateProgress StateAttack::OnStep()
{
	if (m_targetEntity->GetHealth() <= 0)
	{
		// Target extinguished
		m_curProgress = EStateProgress::DONE;
	}

	if (m_curProgress == EStateProgress::IN_PROGRESS && m_isAnimFinished &&
		m_attackComponent->IsReadyToAttack())
	{
		// Direction to target
		AIAgent& agent = GetAgent();
		const Vector2& targetEntityPosition = AIAgentManager::GetInstance()->GetTargetEntity()->GetPos();
		Vector2 toTarget = targetEntityPosition - agent.GetPos();
		toTarget.Normalize();
		m_attackComponent->Attack(toTarget);
		agent.StartAttacking();

		// Start attack animation
		m_isAnimFinished = false;
		m_animComponent->PlayOneShotAnimation(GameConsts::ANIM_TYPE_ATTACK, CC_CALLBACK_0(StateAttack::OnAttackFinished, this));
	}
	return m_curProgress;
}

void StateAttack::OnExit()
{
	m_curProgress = EStateProgress::NONE;
	StopAttack();
}

void StateAttack::OnEventReceived(const String & receivedEvent, const BaseEventData & eventData)
{
	if (receivedEvent == AIAgent::GetEventOnHealthChanged())
	{
		m_curProgress = EStateProgress::FAILED;
	}
}

EAIState StateAttack::GetStateType() const
{
	return EAIState::ATTACK;
}

void StateAttack::OnAttackFinished()
{
	StopAttack();
	PlayIdleAnimation();
	m_curProgress = EStateProgress::DONE;
}

void StateAttack::PlayIdleAnimation()
{
	if (!GetAgent().IsProcessing())
	{
		m_animComponent->PlayLoopingAnimation(AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_IDLE));
	}
}

void StateAttack::StopAttack()
{
	m_isAnimFinished = true;
	GetAgent().StopAttacking();
}

