#pragma once

#include "GenericAttackComponent.h"
#include "World/Projectiles/ProjectileConfig.h"



class RangedAttackComponent: public GenericAttackComponent
{
public:
	static RangedAttackComponent* Create(const ProjectileConfig& config, float range, float secondsBetweenAttacks);

	virtual void Attack(const Vector2& direction) override;

private:
	RangedAttackComponent(const ProjectileConfig& config, float attackRange, float secondsBetweenAttacks);

private:	
	ProjectileConfig	m_projectileConfig;
};


