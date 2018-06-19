#pragma once

#include "AttackComponent.h"
#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class Entity;

class LongSwordAttackComponent : public AttackComponent
{
public:
	static LongSwordAttackComponent*	Create(float secondsBetweenAttacks, float attackRange, float paddingFromBody);	
	virtual void						Attack(const Vector2& direction) override;

private:
	LongSwordAttackComponent(float secondsBetweenAttacks, float attackRange,
		float paddingFromBody);

private:
	bool OnAttackHit(cocos2d::PhysicsWorld& world, 
		cocos2d::PhysicsShape& physicsObject, void* metaData) const;

private:
	const float		m_paddingFromBody;
};

NS_LIGHTSOULS_END