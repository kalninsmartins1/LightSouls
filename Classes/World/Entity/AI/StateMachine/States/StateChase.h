#pragma once


#include "IState.h"
#include "World/Entity/Entity.h"

class AIAgent;

class StateChase: public IState
{
public:
	StateChase(AIAgent& agent);

	virtual void onEnter(AIAnimComponent* pAnimComponent) override;
	virtual StateProgress onStep() override;
	virtual void onExit() override;
	virtual AIState getStateType() override;

private: 

	void onTargetReached();

	StateProgress m_curProgress;
	const Entity& m_targetEntity;
	AIAgent& m_agent;	
};
