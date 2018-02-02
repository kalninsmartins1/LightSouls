#pragma once

#include "World/Entity/Entity.h"
#include "StateMachine/StateMachine.h"


class AIAgent : public Entity
{
public:
	static AIAgent* create(const String& pathToXML);

	virtual void update(float deltaTime) override;

	float getPatrolPause() const;
	float getActiveRadius() const;
	float getAttackRadius() const;
	const String& getType() const;
	const Vector2& getBasePosition() const;	

	void setPatrolPause(float pauseInSeconds);
	void setWorkingRadius(float radius);
	void setAttackRadius(float radius);
	void setBasePosition(const Vector2& position);
	void setAgentType(const String& type);	

private:
	AIAgent();
	bool init(const String& pathToXML);

	StateMachine m_stateMachine;
	Vector2 m_basePosition;
	String m_agentType;
	float m_workingRadius;	
	float m_attackRadius;
	float m_patrolPauseInSeconds;
};
