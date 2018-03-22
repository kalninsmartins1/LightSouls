#include "StateAttack.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"

StateAttack::StateAttack(AIAgent& agent) :
	m_curProgress(StateProgress::NONE),
	m_agent(agent),
	m_targetEntity(AIAgentManager::GetInstance()->getTargetEntity()),
	m_pAttackComponent(nullptr),
	m_pAnimComponent(nullptr),
	m_bIsAttackAnimFinished(true)	
{
}

bool StateAttack::init()
{
	m_pAttackComponent =
		dynamic_cast<AttackComponent*>(
			m_agent.getComponent(ATTACK_COMPONENT));
	CC_ASSERT(m_pAttackComponent != nullptr &&
		"Attack component not found !");

	return m_pAttackComponent != nullptr;
}

void StateAttack::onEnter(AIAnimComponent* pAnimComponent)
{
	m_pAnimComponent = pAnimComponent;
	m_curProgress = StateProgress::IN_PROGRESS;
}

StateProgress StateAttack::onStep()
{
	if(m_curProgress == StateProgress::IN_PROGRESS)
	{
		if(m_pAttackComponent->IsReadyToAttack() && m_bIsAttackAnimFinished)
		{
			cocos2d::Vec2 toTarget = m_targetEntity.getPosition() -
				m_agent.getPosition();
			m_pAttackComponent->Attack(toTarget.getNormalized());

			// Start attack animation
			m_bIsAttackAnimFinished = false;
			m_pAnimComponent->playAttackAnimationWithCallback(
				CC_CALLBACK_0(StateAttack::onAttackFinished, this));
		}
		
		const cocos2d::Vec2 toTarget = m_targetEntity.getPosition() - m_agent.getPosition();
		if(toTarget.length() > m_agent.getAttackRadius())
		{
			// Target run away cant attack
			m_curProgress = StateProgress::DONE;
		}
	}
	return m_curProgress;
}

void StateAttack::onExit()
{
	m_curProgress = StateProgress::NONE;
}

AIState StateAttack::getStateType()
{
	return AIState::ATTACK;
}

void StateAttack::onAttackFinished()
{
	m_bIsAttackAnimFinished = true;	
}
