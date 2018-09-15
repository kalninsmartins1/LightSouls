#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class Entity;
class AIAgent;
enum class EAIState;

class AIAvoidTargetAction : public cocos2d::Action
{
public:
	static AIAvoidTargetAction*	Create(AIAgent& agent);
	virtual void				LoadXMLData(const XMLElement* xmlElement);	
	virtual void				step(float deltaTime) override;
	virtual bool				isDone() const override;

private:
	AIAvoidTargetAction(AIAgent& agent);
	~AIAvoidTargetAction();

private:
	bool						IsTargetTooClose(const Entity* target) const;
	bool						IsTargetFarEnough(const Entity* target) const;

	void						OnCollisionCheck();
	bool						OnRayCastCallback(cocos2d::PhysicsWorld& world, const cocos2d::PhysicsRayCastInfo& info, void* data);
	void						OnAvoidCooldownExpired();
	void						OnAvoidTimerFinished();
	void						StopAvoiding();
	void						StartAvoiding(const Entity* targetEntity);

private:
	AIAgent&		m_agent;
	float			m_stopAvoidingDistance;
	float			m_startAvoidingDistance;
	float			m_collisionCheckInterval;
	bool			m_isAvoiding;
	EAIState		m_skipAvoidState;	
	bool			m_isAvoidCooldownActive;
	bool			m_isAvoidTime;
	float			m_activeTime;
	float			m_cooldownTime;
};

NS_LIGHTSOULS_END