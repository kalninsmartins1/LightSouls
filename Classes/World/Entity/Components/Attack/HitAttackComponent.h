#pragma once

#include "GenericAttackComponent.h"
#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class Entity;

class HitAttackComponent : public GenericAttackComponent
{
public:
	static HitAttackComponent*		Create(float secondsBetweenAttacks, float attackRange);
	virtual void					Attack(const Vector2& direction) override;
	void							RegisterSuccessfulAttack();

private:
	HitAttackComponent(float secondsBetweenAttacks, float attackRange);

private:
	bool OnAttackHit(cocos2d::PhysicsWorld& world, 
		cocos2d::PhysicsShape& physicsObject, void* pMetaData) const;
};

NS_LIGHTSOULS_END