#pragma once

#include "cocos2d.h"

class AttackComponent : public cocos2d::Component
{
public:
	using Vector2 = cocos2d::Vec2;
	using String = std::string;
	
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