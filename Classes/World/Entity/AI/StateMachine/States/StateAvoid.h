#pragma once

#include "LightSoulsTypes.h"
#include "AState.h"

NS_LIGHTSOULS_BEGIN

class Entity;
class GenericAttackComponent;

class StateAvoid : public AState
{
public:
	StateAvoid(AIAgent& agent);

public:
	EAIState					GetStateType() const override;

	virtual void			OnEnter(AnimComponent* animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String& receivedEvent, const AEventData& eventData) override;

private:
	EStateProgress			m_curProgress;	
	GenericAttackComponent* m_attackComponent;
	float					m_startAvoidDistance;
	float					m_stopAvoidDistance;
	bool					m_isAvoiding;
};

NS_LIGHTSOULS_END