#include "CameraFallow.h"
#include "Utils/Utils.h"

CameraFollow* CameraFollow::Create(cocos2d::Node* followedNode)
{
	CameraFollow *follow = new (std::nothrow) CameraFollow(followedNode);

	if (follow != nullptr && followedNode != nullptr)
	{
		follow->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(follow);
	}

	return follow;
}

void CameraFollow::startWithTarget(cocos2d::Node *target)
{
	Action::startWithTarget(target);
	_target->setPosition(m_followNode->getPosition());
}

CameraFollow::CameraFollow(const cocos2d::Node* followNode)
	: m_followNode(followNode)
	, m_cameraState(CameraMoveSate::TARGET_REACHED)
	, m_curSpeed(0.0f)
	, m_maxSpeed(400.0f)
	, m_easeInTime(2.0f)
	, m_easeOutTime(2.0f)	
{

}

bool CameraFollow::isDone() const
{
	return false;
}

void CameraFollow::step(float dt)
{
	Vector2 cameraPos = _target->getPosition();
	Vector2 followedNodePos = m_followNode->getPosition();
	Vector2 toFollowedNode = (followedNodePos - cameraPos);
	_target->setPosition(cameraPos + toFollowedNode * 0.1f * 50 * dt);
}

void CameraFollow::ProcessStateEasingIn(float dt, const Vector2& cameraPos, const Vector2& toFollowNodeNormalized, float distance)
{	
	// Make sure we reach max speed during ease in time
	m_curSpeed += Utils::SafeDevide(m_maxSpeed, m_easeInTime) * dt;

	float moveAmount = m_curSpeed * dt;
	if (m_curSpeed >= m_maxSpeed || distance <= moveAmount)
	{
		CCLOG("Easing In finished !");
		m_curSpeed = m_maxSpeed;
		m_cameraState = CameraMoveSate::FULL_SPEED;
	}
	else
	{
		_target->setPosition(cameraPos + toFollowNodeNormalized * moveAmount);
	}	
}

void CameraFollow::ProcessStateFullSpeed(float dt, const Vector2& cameraPos, const Vector2& toFollowNodeNormalized, float distance)
{	
	if (distance < m_curSpeed)
	{
		CCLOG("Full speed finished !");
		m_cameraState = CameraFollow::CameraMoveSate::EASING_OUT;
	}
	else
	{
		_target->setPosition(cameraPos + toFollowNodeNormalized * m_curSpeed * dt);
	}
}

void CameraFollow::ProcessStateEasingOut(float dt, const Vector2& cameraPos, const Vector2& toFollowNodeNormalized, float distance)
{
	// Cur speed reaches 0 in ease out time	
	m_curSpeed -= Utils::SafeDevide(m_maxSpeed, m_easeOutTime) * dt; 

	float moveAmount = m_curSpeed * dt;
	if (m_curSpeed <= 0 || distance <= moveAmount)
	{
		CCLOG("Easing out finished!");
		m_curSpeed = 0;
		m_cameraState = CameraMoveSate::TARGET_REACHED;
	}
	else
	{
		_target->setPosition(cameraPos + toFollowNodeNormalized * moveAmount);
	}	
}

void CameraFollow::ProcessStateTargetReached(float dt, float distance)
{
	// Wait for distance change
	if (distance > 1)
	{
		Reset();
	}
}

void CameraFollow::Reset()
{
	m_cameraState = CameraMoveSate::EASING_IN;
	m_curSpeed = 0.0f;
	CCLOG("Reset!");
}

