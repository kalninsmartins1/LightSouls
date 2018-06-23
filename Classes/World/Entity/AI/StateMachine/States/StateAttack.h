#pragma once

#include "AState.h"
#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class Entity;
class AttackComponent;

class StateAttack: public AState
{
public:
	StateAttack(AIAgent& agent);

public:		
	AIState					GetStateType() const override;

	virtual void			OnEnter(AnimComponent* animComponent) override;
	virtual StateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String& receivedEvent, const AEventData& eventData) override;

private:
	void					OnAttackFinished();
	void					PlayIdleAnimation();

private:
	StateProgress		m_curProgress;
	AIAgent&			m_agent;
	const Entity*		m_targetEntity;
	AttackComponent*	m_attackComponent;
	AnimComponent*		m_animComponent;
	bool				m_isAnimFinished;
};

NS_LIGHTSOULS_END