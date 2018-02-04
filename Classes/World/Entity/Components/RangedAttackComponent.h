#pragma once

#include "AttackComponent.h"

class RangedAttackComponent: public AttackComponent
{
public:

	static RangedAttackComponent* create(const String& pathToAmmo,
		float maxAmmoFlyDistance, float ammoMoveSpeed,
		float secondsBetweenAttacks);

	virtual void attack(const Vector2& direction) override;

private:
	RangedAttackComponent(const String& pathToAmmo,
		float maxAmmoFlyDistance, float ammoMoveSpeed,
		float secondsBetweenAttacks);	

	String m_pathToAmmo;
	float m_maxAmmoFlyDistance;
	float m_ammoMoveSpeed;
};
