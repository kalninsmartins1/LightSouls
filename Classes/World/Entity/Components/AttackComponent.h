#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class Entity;

class AttackComponent : public cocos2d::Component
{
public:
	AttackComponent(float secondsBetweenAttacks, float attackRange);

	// Returns attack range
	float			GetAttackRange() const;

	// Returns amount of stamina that is consumed
	float			GetStaminaConsumption() const;

	// Returns ready if owner is ready to attack
	virtual bool	IsReadyToAttack() const;

	// Returns the entity that owns this component
	const Entity*	GetOwnerEntity() const;

	void			SetStaminaConsumption(float staminaConsumption);
	virtual void	setOwner(cocos2d::Node* owner);

	// Performs attack
	virtual void	Attack(const Vector2& direction);

private:
	Entity*		m_ownerEntity;
	long long	m_lastTimeAttacked;
	float		m_secondsBetweenAttacks;
	float		m_attackRange;
	float		m_staminaConsumption;
};

NS_LIGHTSOULS_END