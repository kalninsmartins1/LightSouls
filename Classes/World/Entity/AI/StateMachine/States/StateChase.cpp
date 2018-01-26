#include "StateChase.h"

StateChase::StateChase() :
	m_curProgress(StateProgress::NONE)
{
}

void StateChase::onEnter()
{
	m_curProgress = StateProgress::IN_PROGRESS;
}

StateProgress StateChase::onStep()
{
	if(m_curProgress == StateProgress::IN_PROGRESS)
	{
		// Only perform actions while in progress
	}

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
