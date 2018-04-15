#pragma once

#include "IState.h"
#include "cocos2d.h"
#include "World/Entity/Entity.h"

class AIAgent;

class StatePatrol: public IState
{
public:
	using Vector2 = cocos2d::Vec2;

public:
	StatePatrol(AIAgent& agent);

	void OnEnter(AIAnimComponent* pAnimComponent) override;
	StateProgress OnStep() override;
	void OnExit() override;
	AIState GetStateType() override;

private:	
	bool	HasTargetBeenSpotted() const;
	
	void	GetRandomPositionInRange(Vector2& outRandomPosition) const;
	void	StartLookingAround();
	void	OnFinishedLookingAround();

private:
	AIAgent&			m_agent;
	const Entity&		m_targetEntity;
	StateProgress		m_curProgress;
	AIAnimComponent*	m_animComponent;
	Vector2				m_curTargetPosition;
	bool				m_isLookingAround;
};
