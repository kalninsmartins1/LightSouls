#include "CameraShakeTrigger.h"



CameraShakeTrigger::CameraShakeTrigger(const String& triggerEvent, float moveSpeed, float time, float shakeRadius)
	: m_triggerEvent(triggerEvent)
	, m_moveSpeed(moveSpeed)
	, m_time(time)
	, m_shakeRadius(shakeRadius)
{

}

CameraShakeTrigger::CameraShakeTrigger()
	: m_triggerEvent("")
	, m_moveSpeed(0.0f)
	, m_time(0.0f)
	, m_shakeRadius(0.0f)
{
	
}

const String& CameraShakeTrigger::GetTriggerEvent() const
{
	return m_triggerEvent;
}

float CameraShakeTrigger::GetMoveSpeed() const
{
	return m_moveSpeed;
}

float CameraShakeTrigger::GetShakeRadius() const
{
	return m_shakeRadius;
}

float CameraShakeTrigger::GetTime() const
{
	return m_time;
}



