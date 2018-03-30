#pragma once

#include "IState.h"
#include "World/Entity/Entity.h"

class AIAgent;

class StateChase: public IState
{
public:
	StateChase(AIAgent& agent);

	virtual AIState			GetStateType() override;
	
	virtual void			OnEnter(AIAnimComponent* animComponent) override;
	virtual StateProgress	OnStep() override;
	virtual void			OnExit() override;

private: 
	void					OnTargetReached();

	StateProgress	m_curProgress;
	const Entity&	m_targetEntity;
	AIAgent&		m_agent;	
};
