#pragma once

#include "LightSoulsTypes.h"



class CameraFollow: public cocos2d::Action
{
public:
	CameraFollow(const cocos2d::Node* followNode);

public:
	virtual bool isDone() const override;

	static CameraFollow*	Create(cocos2d::Node *followedNode);
	virtual void			startWithTarget(cocos2d::Node *target) override;
	virtual void			step(float dt) override;

private:
	enum class CameraMoveSate
	{
		NONE = 0,
		INIT,
		EASING_IN,
		FULL_SPEED,
		EASING_OUT,
		TARGET_REACHED
	};

private:
	void		ProcessStateEasingIn(float dt, const Vector2& cameraPos, const Vector2& toFollowNodeNormalized, float distance);
	void		ProcessStateFullSpeed(float dt, const Vector2& cameraPos, const Vector2& toFollowNodeNormalized, float distance);
	void		ProcessStateEasingOut(float dt, const Vector2& cameraPos, const Vector2& toFollowNodeNormalized, float distance);
	void		ProcessStateTargetReached(float dt, float distance);
	void		Reset();

private:
	const cocos2d::Node*	m_followNode;
	CameraMoveSate			m_cameraState;
	float					m_curSpeed;
	float					m_maxSpeed;
	float					m_easeInTime;
	float					m_easeOutTime;
};

