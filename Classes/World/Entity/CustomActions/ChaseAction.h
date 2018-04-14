#pragma once

#include "cocos2d.h"
#include "World/Entity/Entity.h"
#include "World/Entity/AI/AIAgentManager.h"

class ChaseAction: public cocos2d::Action
{
public:
	using Vector2 = cocos2d::Vec2;

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
