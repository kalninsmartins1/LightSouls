#pragma once

#include "GenericAttackComponent.h"

class Vector2;
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

