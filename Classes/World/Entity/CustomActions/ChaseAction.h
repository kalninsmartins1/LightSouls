#pragma once

#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

class AIAgent;

class ChaseAction: public cocos2d::Action
{
public:
	ChaseAction(const Entity& chaseTarget, AIAgent& agent);
	virtual ~ChaseAction() override;
	
	virtual bool isDone() const override;		

	static ChaseAction* Create(const Entity& chaseTarget, AIAgent& agent);
	
	virtual void		step(float dt) override;
	
private:
	const Entity&	m_chaseTarget;
	AIAgent&		m_agent;
	bool			m_isDone;
};

NS_LIGHTSOULS_END
