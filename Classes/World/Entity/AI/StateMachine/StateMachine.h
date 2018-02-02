#pragma once

#include "States/IState.h"
#include "States/StateAttack.h"
#include "States/StateChase.h"
#include "States/StatePatrol.h"

class StateMachine
{
public:		
	StateMachine(AIAgent& agent);
	void start();
	void onStep();	

private:	
	void switchState(IState& newState);
	void onStateDone();
	void onStateFailed();

	AIAgent& m_Agent;
	IState* m_curState;
	StateAttack m_attackState;
	StateChase m_chaseState;
	StatePatrol m_patrolState;
};
