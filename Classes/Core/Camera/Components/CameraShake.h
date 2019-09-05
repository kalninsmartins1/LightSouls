/*
  \author Martins Kalnins
  \brief Component for shaking the camera.
*/
#pragma once

#include "Classes/External/CocosEngine.h"
#include "Classes/Core/Math/Vector2.h"

namespace tinyxml2
{
	class XMLElement;
};
class CameraShakeTrigger;
class String;

using XMLElement = tinyxml2::XMLElement;

class CameraShake : public cc::Component
{
public:	
	static CameraShake*		Create(XMLElement* element);
	
	virtual void			update(float deltaTime) override;

private:
	CameraShake();
	~CameraShake();

private:
	bool Init(XMLElement* element);

	void OnStartCameraShake(cocos2d::EventCustom* eventData);
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

