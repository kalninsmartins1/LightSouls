#pragma once

#include "2d/CCSprite.h"

class Entity: public cocos2d::Sprite
{
public:
	typedef cocos2d::Vec2 Vector2;
	typedef std::string String;

	Entity();

	void setBaseMoveSpeed(float moveSpeed);
	void setDodgeSpeed(float dodgeSpeed);
	void setDodgeTime(float dodgeTime);	
	void setMoveDirection(const Vector2& direction);
	void setPhysicsBodySize(cocos2d::Size size);

	float getCurrentMoveSpeed() const;
	Vector2 getHeading() const;	
	cocos2d::Size getPhysicsBodySize() const;

protected:
	Vector2 m_moveDirection;
	cocos2d::Size m_PhysicsBodySize;

	bool m_bIsRuning;
	bool m_bIsDodging;
	bool m_bIsAttacking;

	float m_baseMoveSpeed;
	float m_moveSpeed;
	float m_dodgeSpeed;
	float m_dodgeTime;
};
