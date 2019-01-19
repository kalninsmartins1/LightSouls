#pragma once

#include "LightSoulsTypes.h"

enum class BodyType
{
	NONE,
	BOX,
	EDGE_BOX,
};

class PhysicsBodyConfig
{
public:
	PhysicsBodyConfig();

public:
	const cocos2d::Size&			GetSize() const;
	const cocos2d::PhysicsMaterial& GetPhysicsMaterial() const;
	BodyType						GetBodyType() const;
	int								GetCollisionBitMask() const;
	int								GetCollisionCategory() const;
	bool							IsBodyDynamic() const;
	bool							IsGravityEnabled() const;
	bool							IsRotationEnabled() const;

	void			SetRotationEnabled(bool isEnabled);
	void			SetSize(const cocos2d::Size& size);
	void			SetPhysicsMaterial(const cocos2d::PhysicsMaterial& material);
	void			SetBodyType(const BodyType& type);
	void			SetCollisionBitMask(int collisionBitMask);
	void			SetCollisionCategory(int collisionCategory);
	void			SetIsDynamic(bool isDynamic);
	void			SetIsGravityEnabled(bool isGravityEnabled);

private:
	cocos2d::Size				m_bodySize;
	cocos2d::PhysicsMaterial	m_physicsMaterial;
	BodyType					m_bodyType;
	int							m_collisionBitMask;
	int							m_collisionCategory;
	bool						m_isDynamic;
	bool						m_isGravityEnabled;
	bool						m_isRotationEnabled;
};

