#pragma once

#include "IState.h"
#include "cocos2d.h"
#include "World/Entity/Entity.h"

class AIAgent;

class StatePatrol: public IState
{
public:
	StatePatrol(AIAgent& agent);

	void onEnter(AIAnimComponent* pAnimComponent) override;
	StateProgress onStep() override;
	void onExit() override;
	AIState getStateType() override;
	

private:
	float getTimeToReachTarget(const cocos2d::Vec2& targetPosition) const;
	bool hasTargetBeenSpotted() const;
	void moveToRandomPositionAndWait() const;
	void onFinishedMoving() const;

	AIAgent& m_agent;
	const Entity& m_targetEntity;
	StateProgress m_curProgress;
	AIAnimComponent* m_pAnimComponent;
};
