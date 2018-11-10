#pragma once

#include "LightSoulsTypes.h"

namespace tinyxml2
{
	class XMLElement;
};



class CameraShakeTrigger;

class CameraShake : public cocos2d::Component
{
public:	
	static CameraShake*		Create(tinyxml2::XMLElement* element);
	
	virtual void			update(float deltaTime) override;

private:
	CameraShake();
	~CameraShake();

private:
	bool Init(tinyxml2::XMLElement* element);

	void OnStartCameraShake(cocos2d::EventCustom* eventData);
	void OnEndCameraShake();
	void OnFinishedMoving();

private:
	std::map<String, CameraShakeTrigger>	m_cameraShakeTriggers;
	CameraShakeTrigger*						m_curCameraShakeTrigger;
	bool									m_isCameraShakeActive;
	bool									m_isMovingToNewPosition;
	cocos2d::EventListener*					m_onStartCameraShakeEventListener;
};

