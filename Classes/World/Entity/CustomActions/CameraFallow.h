#pragma once

#include "Classes/External/CocosEngine.h"

class Vector2;

class CameraFollow: public cc::Action
{
public:
	CameraFollow(const cc::Node* followNode);

public:
	virtual bool isDone() const override;

	static CameraFollow*	Create(cc::Node *followedNode);
	virtual void			startWithTarget(cc::Node *target) override;
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
	const cc::Node*				m_followNode;
	CameraMoveSate				m_cameraState;
	float						m_curSpeed;
	float						m_maxSpeed;
	float						m_easeInTime;
	float						m_easeOutTime;
};

