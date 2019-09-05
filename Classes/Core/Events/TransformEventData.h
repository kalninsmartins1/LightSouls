/*
 * \Author Martins Kalnins
 * \Date 15.06.2019
 * \Brief Class for holding event data for position and rotation.
*/
#pragma once

#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/Events/PositionEventData.h"

class TransformEventData : public PositionEventData
{
public:
	TransformEventData(unsigned int senderId, const Vector2& position, const float rotationAngle);

public:
	float GetRotationAngle() const;

private:
	float	m_rotationAngle;
};