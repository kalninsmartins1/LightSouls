#pragma once

#include "AState.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

class StateChase: public AState
{
public:
	StateChase(AIAgent& agent);

public:
	virtual EAIState		GetStateType() const override;
	
	virtual void			OnEnter(AnimComponent* animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String& receivedEvent, const AEventData& eventData) override;	
	virtual void			LoadXMLData(const XMLElement* xmlElement) override;

private:
	EStateProgress	m_curProgress;
	const Entity*	m_targetEntity;
	AnimComponent*	m_animComponent;
	float			m_chaseRadius;
	float			m_chaseStopDistance;
};

NS_LIGHTSOULS_END