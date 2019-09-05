/*
 * \Author Martins Kalnins
 * \Date 06.04.2019
 * \Brief Class for sending position data through event system.
*/
#pragma once

#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/Events/BaseEventData.h"

class PositionEventData : public BaseEventData
{
public:
	PositionEventData(unsigned int senderId, const Vector2& postion);

public:
	const Vector2& GetPosition() const;

private:
	Vector2 m_position;
};