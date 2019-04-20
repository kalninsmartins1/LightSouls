#pragma once

#include "GenericAttackComponent.h"
#include "LightSoulsTypes.h"



class Entity;

class LongSwordAttackComponent : public GenericAttackComponent
{
public:
	static LongSwordAttackComponent*	Create(float secondsBetweenAttacks, float attackRange, float paddingFromBody);	

	void			SetDamageCheckDelay(const float damageCheckDelay);
	virtual void	Attack(const Vector2& direction) override;

protected:
	virtual void OnEntityHit(Entity* hitEntity) const override;

private:
	LongSwordAttackComponent(float secondsBetweenAttacks, float attackRange,
		float paddingFromBody);

private:
	bool OnAttackHit(cocos2d::PhysicsWorld& world, 
		cocos2d::PhysicsShape& physicsObject, void* metaData) const;
	void OnDamageCheck();

private:
	static const String s_eventOnSlash;
	const float			m_paddingFromBody;
	float				m_damageCheckDelay;
	Vector2				m_lastAttackDirection;
};

