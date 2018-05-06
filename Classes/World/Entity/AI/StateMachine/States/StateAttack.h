#pragma once

#include "IState.h"
#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class AIAgent;
class Entity;
class AttackComponent;
class AnimComponent;

class StateAttack: public IState
{
public:
	StateAttack(AIAgent& agent);
	AIState					GetStateType() override;

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
	const Entity&		m_targetEntity;
	AttackComponent*	m_attackComponent;
	AnimComponent*		m_animComponent;
	bool				m_isAnimFinished;
	int					m_curAnimationId;
	const int			m_lastAnimationId;
	const int			m_firstAnimatioId;
};

NS_LIGHTSOULS_END