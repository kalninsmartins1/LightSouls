#pragma once

#include "cocos2d.h"

enum class BodyType
{
	Box,
};

class PhysicsBodyConfig
{
public:
	PhysicsBodyConfig(const cocos2d::Size& size, const cocos2d::PhysicsMaterial&  physicsMaterial, BodyType bodyType,
		int collisionMask, bool isDynamic, bool isGravityEnabled);

	const cocos2d::Size&			GetSize() const;
	const cocos2d::PhysicsMaterial& GetPhysicsMaterial() const;
	BodyType						GetBodyType() const;
	int								GetCollisionBitMask() const;
	bool							IsBodyDynamic() const;
	bool							IsGravityEnabled() const;

private:
	cocos2d::Size m_bodySize;
	cocos2d::PhysicsMaterial m_physicsMaterial;
	BodyType m_bodyType;
	int m_collisionBitMask;
	bool m_isDynamic;
	bool m_isGravityEnabled;
};