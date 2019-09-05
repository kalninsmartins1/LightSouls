#include "TransformEventData.h"

TransformEventData::TransformEventData(unsigned int senderId, const Vector2& position, const float rotationAngle)
	: PositionEventData(senderId, position)
	, m_rotationAngle(rotationAngle)
{

}

float TransformEventData::GetRotationAngle() const
{
	return m_rotationAngle;
}