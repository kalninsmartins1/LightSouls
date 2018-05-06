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
	virtual void			OnEventReceived(const String& receivedEvent, const AEventData& eventData) override;

private:
	StateProgress	m_curProgress;
	const Entity&	m_targetEntity;
	AIAgent&		m_agent;
	AnimComponent*	m_animComponent;
};

NS_LIGHTSOULS_END