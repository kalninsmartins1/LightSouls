#include "CameraShake.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

const String CameraShake::s_eventRequestCameraShake = "EVENT_REQUEST_CAMERA_SHAKE";

CameraShake::CameraShake(float moveSpeed, float time, float shakeRadius)
	: m_moveSpeed(moveSpeed)
	, m_time(time)
	, m_shakeRadius(shakeRadius)
	, m_isCameraShakeActive(false)
	, m_isMovingToNewPosition(false)
{

}

bool CameraShake::Init()
{
	cocos2d::Director::getInstance()->
		getEventDispatcher()->
		addCustomEventListener(s_eventRequestCameraShake,
			CC_CALLBACK_0(CameraShake::OnStartCameraShake, this));

	return true;
}

void CameraShake::OnStartCameraShake()
{
	m_isCameraShakeActive = true;

	// Stop any previous actions
	getOwner()->stopActionByTag(ACTION_CAMERA_SHAKE);

	Utils::StartTimerWithCallback(getOwner(),
		CC_CALLBACK_0(CameraShake::OnEndCameraShake, this), m_time, ACTION_CAMERA_SHAKE);
}

void CameraShake::OnEndCameraShake()
{
	m_isCameraShakeActive = false;
}

void CameraShake::OnFinishedMoving()
{
	m_isMovingToNewPosition = false;
}

const String& CameraShake::GetEventRequestCameraShake()
{
	return s_eventRequestCameraShake;
}

CameraShake* CameraShake::Create(float velocity, float time, float shakeRadius)
{
	CameraShake* cameraShake = new (std::nothrow) CameraShake(velocity, time, shakeRadius);

	if (cameraShake && cameraShake->Init() && cameraShake->init())
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
		Vector2 randomPosition = Utils::GetRandomPositionWithinCircle(cameraPos, m_shakeRadius);
		const Vector3& targetPosition = Vector3(randomPosition.x, randomPosition.y, cameraPos.z);
		
		Vector3 toTargetPositionVec3 = targetPosition - cameraPos;
		const Vector2& toTargetPositionVec2 = Vector2(toTargetPositionVec3.x, toTargetPositionVec3.y);
		const float moveTime = toTargetPositionVec2.length() / m_moveSpeed;

		auto moveTo = cocos2d::MoveTo::create(moveTime, targetPosition);
		auto callback = cocos2d::CallFunc::create(CC_CALLBACK_0(CameraShake::OnFinishedMoving, this));		
		camera->runAction(cocos2d::Sequence::create(moveTo, callback, nullptr));
	}
}

NS_LIGHTSOULS_END