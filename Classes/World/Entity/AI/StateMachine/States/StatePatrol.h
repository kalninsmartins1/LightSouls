#pragma once

#include "AState.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

class StatePatrol: public AState
{
public:
	StatePatrol(AIAgent& agent);

public:
	AIState			GetStateType() const override;
	
	void			OnEnter(AnimComponent* animComponent) override;
	StateProgress	OnStep() override;
	void			OnExit() override;
	virtual void	OnEventReceived(const String& receivedEvent, const AEventData& eventData) override;	

private:	
	bool	HasTargetBeenSpotted() const;
	void	GetRandomPositionInRange(Vector2& outRandomPosition) const;

	void	StartLookingAround();
	void	StartMovingToNewPosition();
	void	OnFinishedLookingAround();

private:
	const Entity*		m_targetEntity;
	AIAgent&			m_agent;
	StateProgress		m_curProgress;
	AnimComponent*		m_animComponent;
	Vector2				m_curTargetPosition;
	bool				m_isLookingAround;
	bool				m_isCollided;
};

NS_LIGHTSOULS_END
