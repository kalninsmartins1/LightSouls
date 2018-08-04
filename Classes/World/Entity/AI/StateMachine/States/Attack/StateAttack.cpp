#include "StateAttack.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "World/Entity/Components/Attack/GenericAttackComponent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "Utils/AnimationUtils.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

StateAttack::StateAttack(AIAgent& agent)
	: AState(agent)	
	, m_curProgress(EStateProgress::NONE)
	, m_targetEntity(nullptr)
	, m_attackComponent(nullptr)
	, m_animComponent(nullptr)
	, m_isAnimFinished(true)
{

}

void StateAttack::OnEnter(AnimComponent* animComponent)
{	
	m_targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	m_animComponent = animComponent;
	m_attackComponent = GetAgent().GetAttackComponent();
	m_curProgress = EStateProgress::IN_PROGRESS;

#if LIGHTSOULS_DEBUG_AI
	CCLOG("StateAttack: OnEnter !");
#endif
}

EStateProgress StateAttack::OnStep()
{
	const Vector2 toTarget = m_targetEntity->getPosition() - GetAgent().getPosition();
	if (toTarget.lengthSquared() > m_attackComponent->GetAttackRangeSqr() && m_isAnimFinished)
	{
		// Target run away cant attack
		m_curProgress = EStateProgress::FAILED;
	}
	else if (m_targetEntity->GetHealth() <= 0)
	{
		// Target extinguished
		m_curProgress = EStateProgress::DONE;
	}

	if(m_curProgress == EStateProgress::IN_PROGRESS && m_isAnimFinished)
	{
		if(m_attackComponent->IsReadyToAttack())
		{
			// Direction to target
			Vector2 toTarget = m_targetEntity->getPosition() - GetAgent().getPosition();
			m_attackComponent->Attack(toTarget.getNormalized());
			GetAgent().StartAttacking();
			
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
	m_curProgress = EStateProgress::NONE;
	m_isAnimFinished = true;

#if LIGHTSOULS_DEBUG_AI
	CCLOG("StateAttack: OnExit !");
#endif
}

void StateAttack::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{
	// ...
}

EAIState StateAttack::GetStateType() const
{
	return EAIState::ATTACK;
}

void StateAttack::OnAttackFinished()
{
	m_isAnimFinished = true;
	GetAgent().StopAttacking();
	PlayIdleAnimation();
	m_curProgress = EStateProgress::DONE;
}

void StateAttack::PlayIdleAnimation()
{
	if (!GetAgent().IsProcessing())
	{
		m_animComponent->PlayLoopingAnimation(AnimationUtils::GetAnimId(ANIM_TYPE_IDLE));
	}
}

NS_LIGHTSOULS_END