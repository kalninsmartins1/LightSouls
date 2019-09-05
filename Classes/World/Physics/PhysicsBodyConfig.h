#pragma once

#include "Classes/External/CocosEngine.h"

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
	void			SetSize(const cc::Size& size);
	void			SetPhysicsMaterial(const cc::PhysicsMaterial& material);
	void			SetBodyType(const BodyType& type);
	void			SetCollisionBitMask(int collisionBitMask);
	void			SetCollisionCategory(int collisionCategory);
	void			SetIsDynamic(bool isDynamic);
	void			SetIsGravityEnabled(bool isGravityEnabled);

private:
	cc::Size					m_bodySize;
	cc::PhysicsMaterial			m_physicsMaterial;
	BodyType					m_bodyType;
	int							m_collisionBitMask;
	int							m_collisionCategory;
	bool						m_isDynamic;
	bool						m_isGravityEnabled;
	bool						m_isRotationEnabled;
};

