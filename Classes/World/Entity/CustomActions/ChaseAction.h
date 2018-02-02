#pragma once

#include "cocos2d.h"
#include "World/Entity/Entity.h"
#include "World/Entity/AI/AIAgentManager.h"

class ChaseAction: public cocos2d::FiniteTimeAction
{
public:
	static ChaseAction* create(const Entity& chaseTarget, AIAgent& agent);

	ChaseAction(const Entity& chaseTarget, AIAgent& agent);

	bool init();
	virtual ~ChaseAction() override;
	virtual bool isDone() const override;		
	virtual void step(float dt) override;
	

private:
	const Entity& m_chaseTarget;
	AIAgent& m_agent;
	bool m_bIsDone;
};
