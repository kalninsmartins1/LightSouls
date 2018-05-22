#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class CameraShake : public cocos2d::Component
{
public:
	static const String&	GetEventRequestCameraShake();
	
	static CameraShake*		Create(float velocity, float time, float shakeRadius);
	
	virtual void			update(float deltaTime) override;

private:
	CameraShake(float moveSpeed, float time, float shakeRadius);

private:
	bool Init();

	void OnStartCameraShake();
	void OnEndCameraShake();
	void OnFinishedMoving();

private:
	static const String	s_eventRequestCameraShake;	
	float				m_moveSpeed;
	float				m_time;
	float				m_shakeRadius;
	bool				m_isCameraShakeActive;
	bool				m_isMovingToNewPosition;
};

NS_LIGHTSOULS_END