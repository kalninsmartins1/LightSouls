#include "StateAttack.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "World/Entity/Components/Attack/AttackComponent.h"
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
{
}

void StateAttack::OnEnter(AnimComponent* animComponent)
{
	m_animComponent = animComponent;
	m_attackComponent = m_agent.GetAttackComponent();
	m_curProgress = StateProgress::IN_PROGRESS;

#if LIGHTSOULS_DEBUG_AI
	CCLOG("StateAttack: OnEnter !");
#endif
}

StateProgress StateAttack::OnStep()
{
	const Vector2 toTarget = m_targetEntity.getPosition() - m_agent.getPosition();
	if (toTarget.length() > m_attackComponent->GetAttackRange() && m_isAnimFinished)
	{
		// Target run away cant attack
		m_curProgress = StateProgress::FAILED;
	}
	else if (m_targetEntity.GetHealth() <= 0)
	{
		// Target extinguished
		m_curProgress = StateProgress::DONE;
	}

	if(m_curProgress == StateProgress::IN_PROGRESS && m_isAnimFinished)
	{
		if(m_attackComponent->IsReadyToAttack())
		{
			// Direction to target
			Vector2 toTarget = m_targetEntity.getPosition() - m_agent.getPosition();
			m_attackComponent->Attack(toTarget.getNormalized());
			m_agent.StartAttacking();
			
			// Check for combo
			if (!m_attackComponent->IsComboExpired())
			{
				m_animComponent->GoToNextAttackAnimation();
			}
			else
			{
				m_animComponent->ResetAttackAnimation();
			}

			// Start attack animation
			m_isAnimFinished = false;
			m_animComponent->PlayAttackAnimation(CC_CALLBACK_0(StateAttack::OnAttackFinished, this));
		}
	}
	return m_curProgress;
}

void StateAttack::OnExit()
{
	m_curProgress = StateProgress::NONE;
	m_isAnimFinished = true;

#if LIGHTSOULS_DEBUG_AI
	CCLOG("StateAttack: OnExit !");
#endif
}

void StateAttack::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{
	// ...
}

AIState StateAttack::GetStateType() const
{
	return AIState::ATTACK;
}

void StateAttack::OnAttackFinished()
{
	m_isAnimFinished = true;
	m_agent.StopAttacking();
	PlayIdleAnimation();
	m_curProgress = StateProgress::DONE;
}

void StateAttack::PlayIdleAnimation()
{
	if (!m_agent.IsProcessing())
	{
		m_animComponent->PlayLoopingAnimation(AnimationUtils::GetAnimId(ANIM_TYPE_IDLE));
	}
}

NS_LIGHTSOULS_END