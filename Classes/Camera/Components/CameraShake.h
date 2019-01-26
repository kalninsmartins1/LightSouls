/*
  \author Martins Kalnins
  \brief Component for shaking the camera.
*/
#pragma once

#include "LightSoulsTypes.h"

class CameraShakeTrigger;

class CameraShake : public cocos2d::Component
{
public:	
	static CameraShake*		Create(XMLElement* element);
	
	virtual void			update(float deltaTime) override;

private:
	CameraShake();
	~CameraShake();

private:
	bool Init(XMLElement* element);

	void OnStartCameraShake(cc::EventCustom* eventData);
	void OnEndCameraShake();
	void OnFinishedMoving();

private:
	std::map<String, CameraShakeTrigger>	m_cameraShakeTriggers;
	CameraShakeTrigger*						m_curCameraShakeTrigger;
	cc::EventListener*						m_onStartCameraShakeEventListener;
	Vector2									m_initialPosition;
	bool									m_isCameraShakeActive;
	bool									m_isMovingToNewPosition;
};

