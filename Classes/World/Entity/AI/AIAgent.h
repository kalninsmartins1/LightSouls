#pragma once

#include "cocos2d.h"
#include "World/Entity/Entity.h"

class StateMachine;

class AIAgent : public Entity
{
public:
	static AIAgent* create(const std::string& pathToXML);
	static AIAgent* create(AIAgent& agent);

	virtual void update(float deltaTime) override;

	float getPatrolPause() const;
	float getActiveRadius() const;
	const std::string& getType() const;
	const cocos2d::Vec2& getBasePosition() const;	

	void setPatrolPause(float pauseInSeconds);
	void setWorkingRadius(float radius);
	void setAttackRadius(float radius);
	void setBasePosition(const cocos2d::Vec2& position);
	void setAgentType(const std::string& type);	

private:
	AIAgent();
	bool init(const std::string& pathToXML);
	bool init(AIAgent& agent);

	std::unique_ptr<StateMachine> m_pStateMachine;
	cocos2d::Vec2 m_basePosition;
	std::string m_agentType;
	float m_workingRadius;	
	float m_attackRadius;
	float m_patrolPauseInSeconds;
};