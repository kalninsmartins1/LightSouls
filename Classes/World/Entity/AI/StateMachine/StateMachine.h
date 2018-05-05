#pragma once

#include "States/StateAttack.h"
#include "States/StateChase.h"
#include "States/StatePatrol.h"

NS_LIGHTSOULS_BEGIN

class AnimComponent;

class StateMachine
{
public:		
	StateMachine(AIAgent& agent);

public:
	// Should be called once to turn on the state machine
	void Start(AnimComponent* animComponent);

	// Should be called periodically to update current state of state machine 
	void OnStep();

	void DispatchEvent(const String& eventType, const AEventData& eventData);

private:	
	void SwitchState(IState& newState);
	void OnStateDone();
	void OnStateFailed();

private:
	AIAgent&			m_agent;
	IState*				m_curState;
	AnimComponent*		m_animComponent;
	StateAttack			m_attackState;
	StateChase			m_chaseState;
	StatePatrol			m_patrolState;
};

NS_LIGHTSOULS_END
