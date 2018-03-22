#pragma once

#include "AttackComponent.h"

class RangedAttackComponent: public AttackComponent
{
public:

	static RangedAttackComponent* Create(const String& pathToAmmo,
		float maxAmmoFlyDistance, float ammoMoveSpeed,
		float secondsBetweenAttacks);

	virtual void Attack(const Vector2& direction) override;

private:
	RangedAttackComponent(const String& pathToAmmo,
		float maxAmmoFlyDistance, float ammoMoveSpeed,
		float secondsBetweenAttacks);	

	String	m_pathToAmmo;
	float	m_maxAmmoFlyDistance;
	float	m_ammoMoveSpeed;
};
