#include "StateAttack.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "World/Entity/Components/AttackComponent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "Utils/AnimationUtils.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

StateAttack::StateAttack(AIAgent& agent)
	: m_curProgress(StateProgress::NONE)
	, m_agent(agent)
	, m_targetEntity(AIAgentManager::GetInstance()->GetTargetEntity())
	, m_attackComponent(nullptr)
	, m_animComponent(nullptr)
	, m_isAnimFinished(true)
	, m_curAnimationId(AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_ONE))
	, m_lastAnimationId(AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_FIVE))
	, m_firstAnimatioId(m_curAnimationId)
{
}

void StateAttack::OnEnter(AnimComponent* animComponent)
{
	m_animComponent = animComponent;
	m_attackComponent = m_agent.GetAttackComponent();
	m_curProgress = StateProgress::IN_PROGRESS;
}

StateProgress StateAttack::OnStep()
{
	if(m_curProgress == StateProgress::IN_PROGRESS && m_isAnimFinished)
	{
		if(m_attackComponent->IsReadyToAttack())
		{
			Vector2 toTarget = m_targetEntity.getPosition() - m_agent.getPosition();
			m_attackComponent->Attack(toTarget.getNormalized());

			// Start attack animation
			m_isAnimFinished = false;
			m_animComponent->PlayOneShotAnimation(m_curAnimationId, CC_CALLBACK_0(StateAttack::OnAttackFinished, this));
		}
		
		const Vector2 toTarget = m_targetEntity.getPosition() - m_agent.getPosition();
		if(toTarget.length() > m_attackComponent->GetAttackRange() && m_isAnimFinished)
		{
			// Target run away cant attack
			m_curProgress = StateProgress::DONE;
		}
	}
	return m_curProgress;
}

void StateAttack::OnExit()
{
	m_curProgress = StateProgress::NONE;
	m_curAnimationId = m_firstAnimatioId;
	m_isAnimFinished = true;
}

void StateAttack::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{
	// ...
}

AIState StateAttack::GetStateType()
{
	return AIState::ATTACK;
}

void StateAttack::OnAttackFinished()
{
	Utils::WrapValue(++m_curAnimationId, m_firstAnimatioId, m_lastAnimationId);
	m_isAnimFinished = true;
	PlayIdleAnimation();
}

void StateAttack::PlayIdleAnimation()
{
	m_animComponent->PlayLoopingAnimation(AnimationUtils::GetAnimId(ANIM_TYPE_IDLE));
}

NS_LIGHTSOULS_END