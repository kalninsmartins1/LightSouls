#pragma once

#include "World/Entity/Entity.h"
#include "World/Entity/AI/StateMachine/StateMachine.h"
#include "Classes/Core/String/String.h"

namespace tinyxml2
{
	class XMLElement;
};

class GenericAttackComponent;
class AIAvoidTargetAction;

using XMLElement = tinyxml2::XMLElement;

class AIAgent : public Entity
{
public:
	~AIAgent();

public:	
	static const char*		 GetEventOnDisappeared();
	static const char*		 GetEventOnHealthChanged();
	static const char*		 GetEventOnDamageTaken();
	float					 GetAttackRange() const;
	float					 GetStoppingDistance() const;
	const String&			 GetType() const;
	const Vector2&			 GetBasePosition() const;
	GenericAttackComponent*	 GetAttackComponent() const;
	EAIState				 GetCurrentAIState() const;
	bool					 IsBackgroundTaskReady() const;

	void SetBasePosition(const Vector2& position);	
	void SetAgentType(const String& type);

	static AIAgent* Create(const String& pathToXML);
	void			Init(const XMLElement* element);
	virtual void	Update(float deltaTime) override;
	void			Reset();

protected:
	virtual void DispatchOnHealthReduceEvent() override;
	virtual void DispatchOnStaminaChangedEvent() const override;
	virtual void DispatchOnGiveDamageEvent() const override;
	virtual void DispatchOnDisappeared() const override;

private:
	AIAgent();

private:
	bool Init(const String& pathToXML);
	bool OnContactBegin(const Vector2& contactPoint, const cocos2d::PhysicsBody* otherBody);

private:	
	static const char*		s_eventAgentHealthChanged;
	static const char*		s_eventAgentDisappeared;
	static const char*		s_eventAgentDamageTaken;
	StateMachine			m_stateMachine;
	GenericAttackComponent*	m_attackComponent;
	Vector2					m_basePosition;	
	String					m_agentType;
	AIAvoidTargetAction*	m_avoidAction;
};


