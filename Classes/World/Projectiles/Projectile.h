#pragma once

#include "LightSoulsTypes.h"
#include "World/Projectiles/ProjectileConfig.h"



class Entity;

class Projectile : public cocos2d::Sprite
{
public:
	Projectile(const Entity& shooter, const ProjectileConfig& config, const Vector2& shootDirection, float attackRange);

public:
	const Entity&		GetShooterEntity() const;
	float				GetDamage() const;

	static Projectile*  Create(const Entity& shooter, const ProjectileConfig& config, const Vector2& shootDirection, float attackRange);
	void				update(float deltaTime) override;
	virtual void		setParent(Node* parent) override;
	void				Destroy();

private:
	bool				Init();
	void				InitProjectilePastFrameAnimation();
	void				RotateProjectileInDirectionOfMovement();
	void				OnSpriteFaded();
	void				StartSpriteFadeOut(Sprite* sprite);

private:
	const Entity&	 m_shooterEntity;
	Sprite*			 m_extraSpriteOne;
	Sprite*			 m_extraSpriteTwo;
	ProjectileConfig m_config;
	Vector2			 m_shootDirection;
	Vector2			 m_startPosition;
	float			 m_attackRange;
	float			 m_damage;
	float			 m_extraSpriteFadeTime;
};


