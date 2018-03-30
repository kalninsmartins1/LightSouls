#pragma once

#include "IState.h"
#include "cocos2d.h"
#include "World/Entity/Entity.h"

class AIAgent;

class StatePatrol: public IState
{
public:
	StatePatrol(AIAgent& agent);

	void OnEnter(AIAnimComponent* pAnimComponent) override;
	StateProgress OnStep() override;
	void OnExit() override;
	AIState GetStateType() override;
	

private:
	float GetTimeToReachTarget(const cocos2d::Vec2& targetPosition) const;
	bool HasTargetBeenSpotted() const;
	void MoveToRandomPositionAndWait() const;
	void OnFinishedMoving() const;

	AIAgent& m_agent;
	const Entity& m_targetEntity;
	StateProgress m_curProgress;
	AIAnimComponent* m_animComponent;
};
