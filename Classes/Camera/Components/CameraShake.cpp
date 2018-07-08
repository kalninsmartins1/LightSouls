#include "CameraShake.h"
#include "Utils/Utils.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils/XML/XMLConsts.h"
#include "Camera\CameraShake\CameraShakeTrigger.h"

NS_LIGHTSOULS_BEGIN

CameraShake::CameraShake()
	: m_cameraShakeTriggers()
	, m_curCameraShakeTrigger(nullptr)
	, m_isCameraShakeActive(false)
	, m_isMovingToNewPosition(false)
	, m_onStartCameraShakeEventListener(nullptr)
{

}

CameraShake::~CameraShake()
{
	cocos2d::Director::getInstance()->
		getEventDispatcher()->removeEventListener(m_onStartCameraShakeEventListener);
}

bool CameraShake::Init(tinyxml2::XMLElement* element)
{
	for (tinyxml2::XMLElement* triggerData = element->FirstChildElement(); triggerData != nullptr;
		triggerData = triggerData->NextSiblingElement())
	{
		// Extract data
		const String& triggerEvent = triggerData->Attribute(XML_TYPE_ATTR);
		const float moveSpeed = triggerData->FloatAttribute(XML_MOVE_SPEED_ATTR);
		const float shakeTime = triggerData->FloatAttribute(XML_TIME_ATTR);
		const float shakeRadius = triggerData->FloatAttribute(XML_CAMERA_SHAKE_RADIUS);

		// Add to collection
		const CameraShakeTrigger trigger(triggerEvent, moveSpeed, shakeTime, shakeRadius);
		m_cameraShakeTriggers[triggerEvent] = trigger;

		// Register for event
		m_onStartCameraShakeEventListener = cocos2d::Director::getInstance()->
		 	getEventDispatcher()->
		 	addCustomEventListener(triggerEvent,
		 		CC_CALLBACK_1(CameraShake::OnStartCameraShake, this));
	}

	return true;
}

void CameraShake::OnStartCameraShake(cocos2d::EventCustom* eventData)
{
	const String& eventName = eventData->getEventName();
	if (!m_isCameraShakeActive && Utils::ContainsKey(m_cameraShakeTriggers, eventName))
	{
		m_curCameraShakeTrigger = &m_cameraShakeTriggers[eventName];
		m_isCameraShakeActive = true;

		// Stop any previous actions
		getOwner()->stopActionByTag(ACTION_CAMERA_SHAKE);

		Utils::StartTimerWithCallback(getOwner(),
			CC_CALLBACK_0(CameraShake::OnEndCameraShake, this),
			m_curCameraShakeTrigger->GetTime(), ACTION_CAMERA_SHAKE);
	}	
}

void CameraShake::OnEndCameraShake()
{
	m_isCameraShakeActive = false;
}

void CameraShake::OnFinishedMoving()
{
	m_isMovingToNewPosition = false;
}

CameraShake* CameraShake::Create(tinyxml2::XMLElement* element)
{
	CameraShake* cameraShake = new (std::nothrow) CameraShake();

	if (cameraShake && cameraShake->Init(element) && cameraShake->init())
	{
		cameraShake->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(cameraShake);
	}

	return cameraShake;
}

void CameraShake::update(float deltaTime)
{
	if (m_isCameraShakeActive && !m_isMovingToNewPosition)
	{
		auto camera = getOwner();
		const Vector3& cameraPos = camera->getPosition3D();
		const float shakeRadius = m_curCameraShakeTrigger->GetShakeRadius();
		Vector2 randomPosition = Utils::GetRandomPositionWithinCircle(cameraPos, shakeRadius);
		const Vector3& targetPosition = Vector3(randomPosition.x, randomPosition.y, cameraPos.z);
		
		Vector3 toTargetPositionVec3 = targetPosition - cameraPos;
		const Vector2& toTargetPositionVec2 = Vector2(toTargetPositionVec3.x, toTargetPositionVec3.y);
		const float moveSpeed = m_curCameraShakeTrigger->GetMoveSpeed();
		const float moveTime = toTargetPositionVec2.length() / moveSpeed;

		auto moveTo = cocos2d::MoveTo::create(moveTime, targetPosition);
		auto callback = cocos2d::CallFunc::create(CC_CALLBACK_0(CameraShake::OnFinishedMoving, this));		
		camera->runAction(cocos2d::Sequence::create(moveTo, callback, nullptr));
	}
}

NS_LIGHTSOULS_END