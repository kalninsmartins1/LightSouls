#include "OnCollisionBeginEventData.h"



OnCollisionBeginEventData::OnCollisionBeginEventData(unsigned int senderId, const String& collidedWithName, const Vector2& collisionPoint)
	: BaseEventData(senderId)
	, m_collidedWithName(collidedWithName)
	, m_collisionPoint(collisionPoint)
{

}

const String& OnCollisionBeginEventData::GetCollidedWithName() const
{
	return m_collidedWithName;
}

const Vector2& OnCollisionBeginEventData::GetCollisionPoint() const
{
	return m_collisionPoint;
}

