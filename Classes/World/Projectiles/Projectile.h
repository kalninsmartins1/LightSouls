#pragma once

#include "LightSoulsTypes.h"
#include "World/Projectiles/ProjectileConfig.h"

NS_LIGHTSOULS_BEGIN

class Entity;

class Projectile : public cocos2d::Sprite
{
public:
	Projectile(const Entity& shooter, const ProjectileConfig& config, const Vector2& shootDirection, float attackRange);

public:
	const Entity&		GetShooterEntity() const;

	static Projectile*  Create(const Entity& shooter, const ProjectileConfig& config, const Vector2& shootDirection, float attackRange);
	void				update(float deltaTime) override;
	void				Destroy();

private:
	bool				Init();
	void				RotateProjectileInDirectionOfMovement();

private:
	const Entity&	 m_shooterEntity;
	ProjectileConfig m_config;
	Vector2			 m_shootDirection;
	Vector2			 m_startPosition;
	float			 m_attackRange;
	bool m_once;
};

NS_LIGHTSOULS_END