#pragma once

#include "World/Entity/Entity.h"
#include "World/Entity/AI/StateMachine/StateMachine.h"

NS_LIGHTSOULS_BEGIN

class GenericAttackComponent;

class AIAgent : public Entity
{
public:
	~AIAgent();

public:	
	static const String&	 GetEventAgentDestroyed();
	static const String&	 GetEventAgentHealthChanged();
	float					 GetAttackRange() const;
	float					 GetStoppingDistance() const;
	const String&			 GetType() const;
	const Vector2&			 GetBasePosition() const;
	GenericAttackComponent*	 GetAttackComponent() const;
	virtual EntityType		 GetEntityType() const override;
	EAIState				 GetCurrentAIState() const;

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
	bool OnContactBegin(const Vector2& contactPoint, const cocos2d::PhysicsBody* otherBody);

private:	
	static String			s_eventAgentHealthChanged;
	static String			s_eventAgentDestroyed;
	StateMachine			m_stateMachine;
	GenericAttackComponent*	m_attackComponent;
	Vector2					m_basePosition;
	Vector2					m_previousPosition;
	String					m_agentType;
};

NS_LIGHTSOULS_END
