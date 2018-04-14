#include "StateAttack.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "World/Entity/Components/AttackComponent.h"

StateAttack::StateAttack(AIAgent& agent) :
	m_curProgress(StateProgress::NONE),
	m_agent(agent),
	m_targetEntity(AIAgentManager::GetInstance()->GetTargetEntity()),
	m_attackComponent(nullptr),
	m_animComponent(nullptr),
	m_isAttackAnimFinished(true)	
{
}

void StateAttack::OnEnter(AIAnimComponent* pAnimComponent)
{
	m_animComponent = pAnimComponent;
	m_attackComponent = m_agent.GetAttackComponent();
	m_curProgress = StateProgress::IN_PROGRESS;
}

StateProgress StateAttack::OnStep()
{
	if(m_curProgress == StateProgress::IN_PROGRESS && m_isAttackAnimFinished)
	{
		if(m_attackComponent->IsReadyToAttack())
		{
			cocos2d::Vec2 toTarget = m_targetEntity.getPosition() -
				m_agent.getPosition();
			m_attackComponent->Attack(toTarget.getNormalized());

			// Start attack animation
			m_isAttackAnimFinished = false;
			m_animComponent->playAttackAnimationWithCallback(
				CC_CALLBACK_0(StateAttack::OnAttackFinished, this));
		}
		
		const cocos2d::Vec2 toTarget = m_targetEntity.getPosition() - m_agent.getPosition();
		if(toTarget.length() > m_attackComponent->GetAttackRange())
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
}

AIState StateAttack::GetStateType()
{
	return AIState::ATTACK;
}

void StateAttack::OnAttackFinished()
{
	m_isAttackAnimFinished = true;	
}
