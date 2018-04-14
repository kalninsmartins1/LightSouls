#include "cocos2d.h"
#include "StateMachine.h"
#include "World/Entity/AI/AIAgent.h"

using namespace cocos2d;

StateMachine::StateMachine(AIAgent& agent) :
	m_agent(agent),	
	m_curState(nullptr),
	m_pAnimComponent(nullptr),
	m_attackState(agent),
	m_chaseState(agent),
	m_patrolState(agent)
{
}

void StateMachine::Start(AIAnimComponent* pAIAnimComponent)
{
	m_pAnimComponent = pAIAnimComponent;
	SwitchState(m_patrolState);
}

void StateMachine::SwitchState(IState& newState)
{
	if(m_curState != nullptr)
	{
		m_curState->OnExit();
	}	
	m_curState = &newState;
	m_curState->OnEnter(m_pAnimComponent);	
}

void StateMachine::OnStep()
{
	const StateProgress& curProgress = m_curState->OnStep();
	switch (curProgress)
	{
	case StateProgress::IN_PROGRESS:
		// Wait for state to finish		
		break;

	case StateProgress::DONE:
		OnStateDone();
			break;

	case StateProgress::FAILED:
		OnStateFailed();
		break;

	default:
		CCLOGERROR("StateMachine: [update] invalid state progress !");
		break;
	}
}


void StateMachine::OnStateDone()
{
	// Switch state
	switch (m_curState->GetStateType())
	{
	case AIState::CHASE:
		SwitchState(m_attackState);
		break;

	case AIState::ATTACK:
		SwitchState(m_patrolState);
		break;

	case AIState::PATROL:
		SwitchState(m_chaseState);
		break;

	default:
		CCLOG("StateMachine: Error [onStateDone] invalid state type !");
		break;
	}
}

void StateMachine::OnStateFailed()
{
	// Switch state
	switch (m_curState->GetStateType())
	{
	case AIState::CHASE:
		SwitchState(m_patrolState);
		break;

	case AIState::ATTACK:
		SwitchState(m_chaseState);
		break;

	case AIState::PATROL:
		SwitchState(m_patrolState);
		break;

	default:
		CCLOG("StateMachine: Error [onStateFailed] invalid state type !");
		break;
	}
}