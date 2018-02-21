#pragma once

#include "AttackComponent.h"

class LongSwordAttackComponent : public AttackComponent
{
public:
	static LongSwordAttackComponent* create(float secondsBetweenAttacks, float attackRange);
	
	virtual void attack(const Vector2& direction) override;	

private:
	LongSwordAttackComponent(float secondsBetweenAttacks, float attackRange);
	bool onAttackHit(cocos2d::PhysicsWorld& world, 
		cocos2d::PhysicsShape& shape, void* pMetaData) const;
	void checkAffectedObjects(const Vector2& direction) const;

	float m_attackRange;
};