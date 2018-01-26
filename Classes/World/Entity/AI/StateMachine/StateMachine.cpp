#include "StateMachine.h"
#include "cocos2d.h"

using namespace cocos2d;

StateMachine::StateMachine(AIAgent& agent) :
	m_Agent(agent),	
	m_curState(nullptr),
	m_patrolState(agent)
{
	// Agent starts out patroling the area
	switchState(m_patrolState);
}

void StateMachine::switchState(IState& newState)
{
	if(m_curState != nullptr)
	{
		m_curState->onExit();
	}	
	m_curState = &newState;
	m_curState->onEnter();
}

void StateMachine::update()
{
	const StateProgress& curProgress = m_curState->onStep();
	switch (curProgress)
	{
	case StateProgress::IN_PROGRESS:
		// Wait for state to finish		
		break;

	case StateProgress::DONE:
		onStateDone();
			break;

	case StateProgress::FAILED:
		onStateFailed();
		break;

	default:
		CCLOGERROR("StateMachine: [update] invalid state progress !");
		break;
	}
}


void StateMachine::onStateDone()
{
	// Switch state
	switch (m_curState->getStateType())
	{
	case AIState::CHASE:
		switchState(m_attackState);
		break;

	case AIState::ATTACK:
		switchState(m_patrolState);
		break;

	case AIState::PATROL:
		switchState(m_chaseState);
		break;

	default:
		CCLOG("StateMachine: Error [onStateDone] invalid state type !");
		break;
	}
}

void StateMachine::onStateFailed()
{
	// Switch state
	switch (m_curState->getStateType())
	{
	case AIState::CHASE:
		switchState(m_patrolState);
		break;

	case AIState::ATTACK:
		switchState(m_chaseState);
		break;

	case AIState::PATROL:
		switchState(m_patrolState);
		break;

	default:
		CCLOG("StateMachine: Error [onStateFailed] invalid state type !");
		break;
	}
}


