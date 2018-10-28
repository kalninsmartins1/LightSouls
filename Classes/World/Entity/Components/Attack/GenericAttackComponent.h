#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class Entity;

class GenericAttackComponent : public cocos2d::Component
{
public:
	GenericAttackComponent(float secondsBetweenAttacks, float attackRange);

public:	
	// Returns attack range
	float			GetAttackRange() const;

	// Returns attack range sqr
	float			GetAttackRangeSqr() const;

	// Returns amount of time before combo expires
	float			GetComboExpireTime() const;	

	// Returns true if combo has expired
	bool			IsComboExpired() const;

	// Returns ready if owner is ready to attack
	virtual bool	IsReadyToAttack() const;

	// Returns the entity that owns this component
	const Entity*	GetOwnerEntity() const;

	void			SetComboExpireTime(float expireTime);	
	virtual void	setOwner(cocos2d::Node* owner);

	static GenericAttackComponent*	Create(float secondsBetweenAttacks, float attackRange);
	// Performs attack
	virtual void					Attack(const Vector2& direction);

protected:
	static void CheckAffectedObjects(const Entity& attacker, const GenericAttackComponent& attackComponent, const Vector2& direction, float paddingFromBody, const cocos2d::PhysicsQueryRectCallbackFunc& callback);
	void		TryToGiveDamage(cocos2d::PhysicsShape& physicsObject) const;

private:
	float			GetSecondsSinceLastAttack() const;

private:
	Entity*		m_ownerEntity;
	long long	m_lastTimeAttacked;
	float		m_secondsBetweenAttacks;
	float		m_comboExpireTime;
	float		m_attackRange;	
};

NS_LIGHTSOULS_END