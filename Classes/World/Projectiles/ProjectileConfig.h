#pragma once

#include "LightSoulsTypes.h"
#include "World/Physics/PhysicsBodyConfig.h"



class ProjectileConfig
{
public:
	ProjectileConfig();

public:
	float						GetMoveSpeed() const;
	const String&				GetPathToSprite() const;
	const PhysicsBodyConfig&	GetPhysicsBodyConfig() const;

	void SetMoveSpeed(float moveSpeed);	
	void SetPathToSprite(const String& pathToSprite);
	void SetPhysicsBodyConfig(const PhysicsBodyConfig& config);

	bool Init(const String& pathToXML);

private:
	float				m_moveSpeed;
	float				m_maxFlyDistance;
	String				m_pathToSprite;
	PhysicsBodyConfig	m_physicsBodyConfig;
};

