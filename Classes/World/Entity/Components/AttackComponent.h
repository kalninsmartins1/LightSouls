#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class AttackComponent : public cocos2d::Component
{
public:
	AttackComponent(float secondsBetweenAttacks, float attackRange);

	// Returns attack range
	float			GetAttackRange() const;

	// Returns ready if owner is ready to attack
	virtual bool	IsReadyToAttack() const;
	
	// Performs attack
	virtual void	Attack(const Vector2& direction);

private:
	long long	m_lastTimeAttacked;
	float		m_secondsBetweenAttacks;
	float		m_attackRange;
};

NS_LIGHTSOULS_END