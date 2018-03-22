#pragma once

#include "AttackComponent.h"
#include "World/Entity/Entity.h"

class LongSwordAttackComponent : public AttackComponent
{
public:
	static LongSwordAttackComponent* create(float secondsBetweenAttacks, float attackRange, float paddingFromBody);
	
	virtual void Attack(const Vector2& direction) override;
	virtual void setOwner(cocos2d::Node* owner) override;
private:
	LongSwordAttackComponent(float secondsBetweenAttacks, float attackRange,
		float paddingFromBody);
	bool OnAttackHit(cocos2d::PhysicsWorld& world, 
		cocos2d::PhysicsShape& shape, void* pMetaData) const;
	void CheckAffectedObjects(const Vector2& direction) const;

	const Entity* m_ownerEntity;
	const float m_attackRange;
	const float m_paddingFromBody;
};