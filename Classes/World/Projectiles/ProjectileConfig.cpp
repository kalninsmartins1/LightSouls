#include "ProjectileConfig.h"
#include "Utils/XML/XMLLoader.h"



ProjectileConfig::ProjectileConfig()
	: m_moveSpeed(0.0f)
	, m_maxFlyDistance(0.0f)
	, m_pathToSprite()
{

}

float ProjectileConfig::GetMoveSpeed() const
{
	return m_moveSpeed;
}

const String& ProjectileConfig::GetPathToSprite() const
{
	return m_pathToSprite;
}

const PhysicsBodyConfig& ProjectileConfig::GetPhysicsBodyConfig() const
{
	return m_physicsBodyConfig;
}

void ProjectileConfig::SetMoveSpeed(float moveSpeed)
{
	m_moveSpeed = moveSpeed;
}

void ProjectileConfig::SetPathToSprite(const String& pathToSprite)
{
	m_pathToSprite = pathToSprite;
}

void ProjectileConfig::SetPhysicsBodyConfig(const PhysicsBodyConfig& config)
{
	m_physicsBodyConfig = config;
}

bool ProjectileConfig::Init(const String& pathToXML)
{
	return XMLLoader::InitializeProjectileConfig(*this, pathToXML);
}


