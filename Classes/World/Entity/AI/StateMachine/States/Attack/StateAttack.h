#pragma once

#include "../AState.h"



class Entity;
class GenericAttackComponent;

class StateAttack: public AState
{
public:
	StateAttack(AIAgent& agent);

public:		
	EAIState					GetStateType() const override;

	virtual void			OnEnter(AnimComponent& animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String& receivedEvent, const AEventData& eventData) override;

private:
	void					OnAttackFinished();
	void					PlayIdleAnimation();
	void					StopAttack();

private:
	EStateProgress			m_curProgress;	
	const Entity*			m_targetEntity;
	GenericAttackComponent*	m_attackComponent;
	AnimComponent*			m_animComponent;
	bool					m_isAnimFinished;
};

