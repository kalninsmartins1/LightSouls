#pragma once

#include "IState.h"

class AIAgent;
class Entity;
class AttackComponent;

class StateAttack: public IState
{
public:
	StateAttack(AIAgent& agent);
	AIState					GetStateType() override;

	bool					Init();
	virtual void			OnEnter(AIAnimComponent* animComponent) override;
	virtual StateProgress	OnStep() override;
	virtual void			OnExit() override;

private:

	void					OnAttackFinished();

	StateProgress		m_curProgress;
	AIAgent&			m_agent;
	const Entity&		m_targetEntity;
	AttackComponent*	m_attackComponent;
	AIAnimComponent*	m_animComponent;
	bool				m_isAttackAnimFinished;
};
