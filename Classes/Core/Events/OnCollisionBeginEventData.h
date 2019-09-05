#pragma once

#include <string>
#include "BaseEventData.h"
#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/String/String.h"

class OnCollisionBeginEventData : public BaseEventData
{
public:
	OnCollisionBeginEventData(unsigned int senderId, const String& collidedWithName, const Vector2& collisionPoint);

public:
	const String&			GetCollidedWithName() const;
	const Vector2&	GetCollisionPoint() const;

private:
	String			m_collidedWithName;
	Vector2	m_collisionPoint;
};

