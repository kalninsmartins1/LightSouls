#include "StateAttack.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"

StateAttack::StateAttack(AIAgent& agent) :
	m_curProgress(StateProgress::NONE),
	m_agent(agent),
	m_targetEntity(AIAgentManager::getInstance()->getTargetEntity())
{
}

bool StateAttack::init()
{
	m_pAttackComponent =
		dynamic_cast<RangedAttackComponent*>(
			m_agent.getComponent(RANGED_ATTACK_COMPONENT));
	CC_ASSERT(m_pAttackComponent != nullptr &&
		"Attack component not found !");

	return m_pAttackComponent != nullptr;
}

void StateAttack::onEnter()
{
	m_curProgress = StateProgress::IN_PROGRESS;
}

StateProgress StateAttack::onStep()
{
	if(m_curProgress == StateProgress::IN_PROGRESS)
	{
		if(m_pAttackComponent->isReadyToAttack())
		{
			Vector2 toTarget = m_targetEntity.getPosition() -
				m_agent.getPosition();
			m_pAttackComponent->shoot(toTarget.getNormalized());
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
