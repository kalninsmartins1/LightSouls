#include "OnCollisionBeginEventData.h"

NS_LIGHTSOULS_BEGIN

OnCollisionBeginEventData::OnCollisionBeginEventData(unsigned int receiverId, const String& collidedWithName)
	: AEventData(receiverId)
	, m_collidedWithName(collidedWithName)
{
}

const String& OnCollisionBeginEventData::GetCollidedWithName() const
{
	return m_collidedWithName;
}

NS_LIGHTSOULS_END
