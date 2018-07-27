#include "Projectile.h"
#include "ProjectileConfig.h"
#include "World/Physics/PhysicsManager.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

Projectile::Projectile(const Entity& shooter, const ProjectileConfig& config, const Vector2& shootDirection, float attackRange)
	: m_shooterEntity(shooter)
	, m_config(config)	
	, m_shootDirection(shootDirection)
	, m_startPosition(shooter.getPosition())
	, m_attackRange(attackRange)
{
	
}

const Entity& Projectile::GetShooterEntity() const
{
	return m_shooterEntity;
}

Projectile* Projectile::Create(const Entity& shooter, const ProjectileConfig& config, const Vector2& shootDirection, float attackRange)
{
	Projectile* projectile = new (std::nothrow) Projectile(shooter, config, shootDirection, attackRange);
	if (projectile != nullptr && projectile->Init())
	{
		projectile->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(projectile);
	}

	return projectile;
}

bool Projectile::Init()
{
	PhysicsManager::AddPhysicsBody(*this, m_config.GetPhysicsBodyConfig());
	setPosition(m_startPosition);
	return initWithFile(m_config.GetPathToSprite());
}

void Projectile::update(float deltaTime)
{
	const Vector2& curPosition = getPosition();
	float distanceCoveredSqrt = m_startPosition.distanceSquared(curPosition);
	if (distanceCoveredSqrt > (m_attackRange*m_attackRange))
	{
		Destroy();
	}
	else
	{
		setPosition(curPosition + m_shootDirection * 
			m_config.GetMoveSpeed() * deltaTime);
		RotateProjectileInDirectionOfMovement();
	}
}

void Projectile::Destroy()
{
	_parent->removeChild(this);
}

void Projectile::RotateProjectileInDirectionOfMovement()
{	
	const float angleBetweenVectors = CC_RADIANS_TO_DEGREES(acos(
		Vector2(0, 1).dot(m_shootDirection) / m_shootDirection.length()));
	setRotation(angleBetweenVectors);
}

NS_LIGHTSOULS_END