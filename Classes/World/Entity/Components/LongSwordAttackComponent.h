#pragma once

#include "AttackComponent.h"
#include "World/Entity/Entity.h"

class LongSwordAttackComponent : public AttackComponent
{
public:	
	virtual void setOwner(cocos2d::Node* owner) override;
	
	static LongSwordAttackComponent*	Create(float secondsBetweenAttacks, float attackRange, float paddingFromBody);
	virtual void						Attack(const Vector2& direction) override;

private:
	LongSwordAttackComponent(float secondsBetweenAttacks, float attackRange,
		float paddingFromBody);
	
	bool OnAttackHit(cocos2d::PhysicsWorld& world, 
		cocos2d::PhysicsShape& shape, void* pMetaData) const;
	void CheckAffectedObjects(const Vector2& direction) const;

private:
	const Entity*	m_ownerEntity;
	const float		m_paddingFromBody;
};