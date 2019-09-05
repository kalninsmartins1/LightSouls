#include "CameraShake.h"
#include "Utils/Utils.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils/XML/XMLConsts.h"
#include "Classes/Core/Camera/CameraShake/CameraShakeTrigger.h"

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
	cc::Director::getInstance()->
		getEventDispatcher()->removeEventListener(m_onStartCameraShakeEventListener);
}

bool CameraShake::Init(tinyxml2::XMLElement* element)
{
	for (tinyxml2::XMLElement* triggerData = element->FirstChildElement(); triggerData != nullptr;
		triggerData = triggerData->NextSiblingElement())
	{
		// Extract data
		const String& triggerEvent = triggerData->Attribute(XMLConsts::TYPE_ATTR);
		const float moveSpeed = triggerData->FloatAttribute(XMLConsts::MOVE_SPEED_ATTR);
		const float shakeTime = triggerData->FloatAttribute(XMLConsts::TIME_ATTR);
		const float shakeRadius = triggerData->FloatAttribute(XMLConsts::CAMERA_SHAKE_RADIUS);

		// Add to collection
		const CameraShakeTrigger trigger(triggerEvent, moveSpeed, shakeTime, shakeRadius);
		m_cameraShakeTriggers[triggerEvent] = trigger;

		// Register for event
		m_onStartCameraShakeEventListener = cocos2d::Director::getInstance()->
		 	getEventDispatcher()->
		 	addCustomEventListener(triggerEvent.GetCStr(),
		 		CC_CALLBACK_1(CameraShake::OnStartCameraShake, this));

		cc::Node* owner = getOwner();
		if (owner != nullptr)
		{
			m_initialPosition = Vector2(owner->getPositionX(), owner->getPositionY());
		}
	}

	return true;
}

void CameraShake::OnStartCameraShake(cc::EventCustom* eventData)
{
	String eventName = "";
	if (eventData != nullptr)
	{
		eventName = eventData->getEventName();
	}
	
	if (!m_isCameraShakeActive && 
		!eventName.IsEmpty() &&
		Utils::ContainsKey(m_cameraShakeTriggers, eventName))
	{
		m_curCameraShakeTrigger = &m_cameraShakeTriggers[eventName];
		m_isCameraShakeActive = true;

		// Stop any previous actions
		cc::Node* owner = getOwner();
		if (owner != nullptr)
		{
			owner->stopActionByTag(GameConsts::ACTION_CAMERA_SHAKE);

			Utils::StartTimerWithCallback(owner,
				CC_CALLBACK_0(CameraShake::OnEndCameraShake, this),
				m_curCameraShakeTrigger->GetTime(), GameConsts::ACTION_CAMERA_SHAKE);
		}
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
	cc::Node* camera = getOwner();
	if (camera != nullptr && m_curCameraShakeTrigger != nullptr)
	{
		if (m_isCameraShakeActive && !m_isMovingToNewPosition)
		{
			const Vector3& cameraPos = camera->getPosition3D();
			const float shakeRadius = m_curCameraShakeTrigger->GetShakeRadius();
			Vector2 randomPosition = Utils::GetRandomPositionWithinCircle(cameraPos, shakeRadius);
			const Vector3& targetPosition = Vector3(randomPosition.GetX(), randomPosition.GetY(), cameraPos.z);

			Vector3 toTargetPositionVec3 = targetPosition - cameraPos;
			const Vector2& toTargetPositionVec2 = Vector2(toTargetPositionVec3.x, toTargetPositionVec3.y);
			const float moveSpeed = m_curCameraShakeTrigger->GetMoveSpeed();
			const float moveTime = toTargetPositionVec2.GetLenght() / moveSpeed;

			auto moveTo = cocos2d::MoveTo::create(moveTime, targetPosition);
			auto callback = cocos2d::CallFunc::create(CC_CALLBACK_0(CameraShake::OnFinishedMoving, this));
			camera->runAction(cocos2d::Sequence::create(moveTo, callback, nullptr));
		}
		else if (!m_isCameraShakeActive)
		{
			// When camera is not shaking move back to initial position
			auto pos = camera->getPosition();
			Vector2 cameraPos(pos.x, pos.y);
			Vector2 toInitialPosition = m_initialPosition - cameraPos;
			float moveSpeed = m_curCameraShakeTrigger->GetMoveSpeed();

			Vector2 finalPos = cameraPos + toInitialPosition * 0.1f * moveSpeed * deltaTime;
			camera->setPosition(finalPos.GetX(), finalPos.GetY());
		}
	}	
}

