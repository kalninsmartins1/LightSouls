#pragma once

#include "AttackComponent.h"
#include "World/Entity/Entity.h"

class LongSwordAttackComponent : public AttackComponent
{
public:
	static LongSwordAttackComponent* create(float secondsBetweenAttacks, float attackRange, float paddingFromBody);
	
	virtual void attack(const Vector2& direction) override;
	virtual void setOwner(cocos2d::Node* owner) override;
private:
	LongSwordAttackComponent(float secondsBetweenAttacks, float attackRange,
		float paddingFromBody);
	bool onAttackHit(cocos2d::PhysicsWorld& world, 
		cocos2d::PhysicsShape& shape, void* pMetaData) const;
	void checkAffectedObjects(const Vector2& direction) const;

	const Entity* m_pOwnerEntity;
	const float m_attackRange;
	const float m_paddingFromBody;
};