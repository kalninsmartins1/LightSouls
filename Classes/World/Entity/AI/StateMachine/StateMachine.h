#pragma once

#include "States/IState.h"
#include "States/StateAttack.h"
#include "States/StateChase.h"
#include "States/StatePatrol.h"
#include "World/Entity/Components/AIAnimComponent.h"

class StateMachine
{
public:		
	StateMachine(AIAgent& agent);

	// Should be called once to turn on the state machine
	void start(AIAnimComponent* pAIAnimComponent);

	// Should be called periodically to update current state of state machine 
	void onStep();	

private:	
	void switchState(IState& newState);
	void onStateDone();
	void onStateFailed();

	AIAgent& m_agent;
	IState* m_curState;
	AIAnimComponent*  m_pAnimComponent;
	StateAttack m_attackState;
	StateChase m_chaseState;
	StatePatrol m_patrolState;
};
