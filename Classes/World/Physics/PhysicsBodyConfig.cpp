#include "PhysicsBodyConfig.h"

PhysicsBodyConfig::PhysicsBodyConfig(const cocos2d::Size& size, const cocos2d::PhysicsMaterial& physicsMaterial, BodyType bodyType, int collisionMask, bool isDynamic, bool isGravityEnabled)
	: m_bodySize(size)
	, m_physicsMaterial(physicsMaterial)
	, m_bodyType(bodyType)
	, m_collisionBitMask(collisionMask)
	, m_isDynamic(isDynamic)
	, m_isGravityEnabled(isGravityEnabled)
{

}

const cocos2d::Size& PhysicsBodyConfig::GetSize() const
{
	return m_bodySize;
}

const cocos2d::PhysicsMaterial& PhysicsBodyConfig::GetPhysicsMaterial() const
{
	return m_physicsMaterial;
}

BodyType PhysicsBodyConfig::GetBodyType() const
{
	return m_bodyType;
}

int PhysicsBodyConfig::GetCollisionBitMask() const
{
	return m_collisionBitMask;
}

bool PhysicsBodyConfig::IsBodyDynamic() const
{
	return m_isDynamic;
}

bool PhysicsBodyConfig::IsGravityEnabled() const
{
	return m_isGravityEnabled;
}

bool PhysicsBodyConfig::IsRotationEnabled() const
{
	return m_isRotationEnabled;
}

void PhysicsBodyConfig::SetRotationEnabled(bool isEnabled)
{
	m_isRotationEnabled = isEnabled;
}

