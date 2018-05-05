#pragma once

#include "World/Entity/Entity.h"
#include "World/Entity/AI/StateMachine/StateMachine.h"

NS_LIGHTSOULS_BEGIN

class AttackComponent;

class AIAgent : public Entity
{
public:
	float				GetPatrolPause() const;
	float				GetPatrolRadius() const;
	float				GetChaseRadius() const;
	float				GetChaseStopDistance() const;
	float				GetAttackRange() const;
	float				GetStoppingDistance() const;
	const String&		GetType() const;
	const Vector2&		GetBasePosition() const;
	AttackComponent*	GetAttackComponent() const;	

	void SetPatrolPause(float pauseInSeconds);
	void SetPatrolRadius(float radius);
	void SetChaseRadius(float radius);
	void SetChaseStopDistance(float distance);
	void SetBasePosition(const Vector2& position);
	void SetAgentType(const String& type);

	static AIAgent* Create(const String& pathToXML);
	virtual void	update(float deltaTime) override;

private:
	AIAgent();
	bool Init(const String& pathToXML);
	void OnContactBegin(const cocos2d::PhysicsBody* otherBody);	

private:	
	StateMachine	    m_stateMachine;
	AttackComponent*	m_attackComponent;
	Vector2				m_basePosition;
	Vector2				m_previousPosition;
	String				m_agentType;
	float				m_chaseRadius;
	float			    m_patrolRadius;	
	float				m_patrolPauseInSeconds;
	float				m_chaseStopDistance;
};

NS_LIGHTSOULS_END
