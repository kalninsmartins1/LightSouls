#pragma once

#include "AttackComponent.h"
#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class Entity;

class HitAttackComponent : public AttackComponent
{
public:
	static HitAttackComponent*	Create(float secondsBetweenAttacks, float attackRange);
	virtual void					Attack(const Vector2& direction) override;

private:
	HitAttackComponent(float secondsBetweenAttacks, float attackRange);

private:
	bool OnAttackHit(cocos2d::PhysicsWorld& world, 
		cocos2d::PhysicsShape& physicsObject, void* pMetaData) const;
};

NS_LIGHTSOULS_END