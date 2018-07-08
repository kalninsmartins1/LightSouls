#pragma once

#include "AState.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

class StateChase: public AState
{
public:
	StateChase(AIAgent& agent);

public:
	virtual EAIState			GetStateType() const override;
	
	virtual void			OnEnter(AnimComponent* animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String& receivedEvent, const AEventData& eventData) override;	

private:
	EStateProgress	m_curProgress;
	const Entity*	m_targetEntity;
	AIAgent&		m_agent;
	AnimComponent*	m_animComponent;
};

NS_LIGHTSOULS_END