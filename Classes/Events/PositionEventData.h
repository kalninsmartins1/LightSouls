/*
 * \Author Martins Kalnins
 * \Date 06.04.2019
 * \Brief Class for sending position data through event system.
*/
#pragma once

#include "LightSoulsTypes.h"
#include "Classes/Events/BaseEventData.h"

class PositionEventData : public BaseEventData
{
public:
	PositionEventData(unsigned int senderId, const Vector2& spawnPos);

public:
	const Vector2& GetPosition() const;

private:
	Vector2 m_position;
};