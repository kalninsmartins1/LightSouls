#pragma once

#include "GenericAttackComponent.h"
#include "LightSoulsTypes.h"



class Entity;

class LongSwordAttackComponent : public GenericAttackComponent
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

