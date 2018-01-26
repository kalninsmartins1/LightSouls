#pragma once
#include "IState.h"
#include "World/Entity/AI/AIAgent.h"

class StatePatrol: public IState
{
public:
	StatePatrol(AIAgent& agent);

	void onEnter() override;
	StateProgress onStep() override;
	void onExit() override;
	AIState getStateType() override;
	

private:
	float getTimeToReachTarget(const cocos2d::Vec2& targetPosition) const;
	bool hasPlayerBeenSpotted() const;
	void moveToRandomPosition();

	AIAgent& m_agent;
	const Entity& m_targetEntity;
	StateProgress m_curProgress;
};
