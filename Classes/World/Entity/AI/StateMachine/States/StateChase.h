#pragma once

#include "IState.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

class AIAgent;

class StateChase: public IState
{
public:
	StateChase(AIAgent& agent);

public:
	virtual AIState			GetStateType() override;
	
	virtual void			OnEnter(AnimComponent* animComponent) override;
	virtual StateProgress	OnStep() override;
	virtual void			OnExit() override;

private: 
	void					OnTargetReached();

private:
	StateProgress	m_curProgress;
	const Entity&	m_targetEntity;
	AIAgent&		m_agent;	
};

NS_LIGHTSOULS_END