#pragma once

#include "World/Entity/Entity.h"
#include "World/Entity/AI/StateMachine/StateMachine.h"

namespace tinyxml2
{
	class XMLElement;
};

NS_LIGHTSOULS_BEGIN

class AttackComponent;

class AIAgent : public Entity
{
public:
	~AIAgent();

public:
	static const String& GetEventAgentDestroyed();
	static const String& GetEventAgentHealthChanged();
	float				 GetPatrolPause() const;
	float				 GetPatrolRadius() const;
	float				 GetChaseRadius() const;
	float				 GetChaseStopDistance() const;
	float				 GetAttackRange() const;
	float				 GetStoppingDistance() const;
	const String&		 GetType() const;
	const Vector2&		 GetBasePosition() const;
	AttackComponent*	 GetAttackComponent() const;	

	void SetPatrolPause(float pauseInSeconds);
	void SetPatrolRadius(float radius);
	void SetChaseRadius(float radius);
	void SetChaseStopDistance(float distance);
	void SetBasePosition(const Vector2& position);
	void SetAgentType(const String& type);

	static AIAgent* Create(const String& pathToXML);
	void			Init(const tinyxml2::XMLElement* element);
	virtual void	update(float deltaTime) override;	
	void			Reset();

protected:
	virtual void DispatchOnHealthChangedEvent() override;
	virtual void DispatchOnStaminaChangedEvent() const override;
	virtual void DispatchOnGiveDamageEvent() const override;

private:
	AIAgent();

private:
	bool Init(const String& pathToXML);
	bool OnContactBegin(const cocos2d::PhysicsBody* otherBody);	

private:
	static String		s_eventAgentHealthChanged;
	static String		s_eventAgentDestroyed;
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
