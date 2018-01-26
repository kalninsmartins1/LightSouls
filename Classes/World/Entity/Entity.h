#pragma once
#include "2d/CCSprite.h"


class Entity: public cocos2d::Sprite
{
public:
	Entity();

	void setBaseMoveSpeed(float moveSpeed);
	void setDodgeSpeed(float dodgeSpeed);
	void setDodgeTime(float dodgeTime);	

	float getCurrentMoveSpeed() const;

	cocos2d::Vec2 getHeading() const;

protected:
	cocos2d::Vec2 m_moveDirection;

	bool m_isRuning;
	bool m_bIsDodging;
	bool m_bIsAttacking;

	float m_baseMoveSpeed;
	float m_moveSpeed;
	float m_dodgeSpeed;
	float m_dodgeTime;
};
