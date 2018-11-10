#pragma once

#include "LightSoulsTypes.h"
#include "AEventData.h"



class OnCollisionBeginEventData : public AEventData
{
public:
	OnCollisionBeginEventData(unsigned int receiverId, const String& collidedWithName, const Vector2& collisionPoint);

public:
	const String&	GetCollidedWithName() const;
	const Vector2&	GetCollisionPoint() const;

private:
	String	m_collidedWithName;
	Vector2 m_collisionPoint;
};

