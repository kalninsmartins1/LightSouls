#pragma once

#include "AttackComponent.h"

NS_LIGHTSOULS_BEGIN

class RangedAttackComponent: public AttackComponent
{
public:
	static RangedAttackComponent* Create(const String& pathToAmmo,
		float attackRange, float ammoMoveSpeed,
		float secondsBetweenAttacks);

	virtual void Attack(const Vector2& direction) override;

private:
	RangedAttackComponent(const String& pathToAmmo,
		float maxAmmoFlyDistance, float ammoMoveSpeed,
		float secondsBetweenAttacks);	

private:
	String	m_pathToAmmo;
	float	m_ammoMoveSpeed;
};

NS_LIGHTSOULS_END
