/*
 \author Martins Kalnins
 \brief POD class for CameraShake component.
*/
#pragma once

#include "Classes/Core/String/String.h"

class CameraShakeTrigger
{
public:
	CameraShakeTrigger();
	CameraShakeTrigger(const String& triggerEvent, float moveSpeed, float time, float shakeRadius);

public:
	const String&	GetTriggerEvent() const;
	float			GetMoveSpeed() const;
	float			GetTime() const;
	float			GetShakeRadius() const;

private:
	String				m_triggerEvent;
	float				m_moveSpeed;
	float				m_time;
	float				m_shakeRadius;
};


