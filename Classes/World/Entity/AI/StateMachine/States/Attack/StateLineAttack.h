#pragma once

#include "../AState.h"

NS_LIGHTSOULS_BEGIN

class GenericAttackComponent;
class Entity;

class StateLineAttack : public AState
{
public:
	StateLineAttack(AIAgent& aiAgent);

public:
	virtual EAIState		GetStateType() const override;	

	virtual void			OnEnter(AnimComponent * animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String & receivedEvent, const AEventData & eventData) override;	

private:
	EStateProgress			m_curProgress;
	AIAgent&				m_agent;
	Entity*					m_targetEntity;
	Vector2					m_targetPosition;
	GenericAttackComponent*	m_attackComponent;
};

NS_LIGHTSOULS_END