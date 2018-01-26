#include "StateAttack.h"

StateAttack::StateAttack() :
	m_curProgress(StateProgress::NONE)
{
}

void StateAttack::onEnter()
{
	m_curProgress = StateProgress::IN_PROGRESS;
}

StateProgress StateAttack::onStep()
{
	if(m_curProgress == StateProgress::IN_PROGRESS)
	{
		// Do stuff only while in progress		
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
