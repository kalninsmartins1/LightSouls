#include "PositionEventData.h"

PositionEventData::PositionEventData(unsigned int senderId, const Vector2& position)
	: AEventData(senderId)
	, m_position(position)
{

}

const Vector2& PositionEventData::GetPosition() const
{
	return m_position;
}
