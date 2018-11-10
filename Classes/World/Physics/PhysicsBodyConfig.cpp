#include "PhysicsBodyConfig.h"



PhysicsBodyConfig::PhysicsBodyConfig()
	: m_bodySize(cocos2d::Size::ZERO)
	, m_physicsMaterial()
	, m_bodyType(BodyType::NONE)
	, m_collisionBitMask(-1)
	, m_collisionCategory(-1)
	, m_isDynamic(false)
	, m_isGravityEnabled(false)
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

int PhysicsBodyConfig::GetCollisionCategory() const
{
	return m_collisionCategory;
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

void PhysicsBodyConfig::SetSize(const cocos2d::Size& size)
{
	m_bodySize = size;
}

void PhysicsBodyConfig::SetPhysicsMaterial(const cocos2d::PhysicsMaterial& material)
{
	m_physicsMaterial = material;
}

void PhysicsBodyConfig::SetBodyType(const BodyType& type)
{
	m_bodyType = type;
}

void PhysicsBodyConfig::SetCollisionBitMask(int collisionBitMask)
{
	m_collisionBitMask = collisionBitMask;
}

void PhysicsBodyConfig::SetCollisionCategory(int collisionCategory)
{
	m_collisionCategory = collisionCategory;
}

void PhysicsBodyConfig::SetIsDynamic(bool isDynamic)
{
	m_isDynamic = isDynamic;
}

void PhysicsBodyConfig::SetIsGravityEnabled(bool isGravityEnabled)
{
	m_isGravityEnabled = isGravityEnabled;
}

