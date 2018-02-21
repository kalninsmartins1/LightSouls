#pragma once

#include "cocos2d.h"

class AttackComponent : public cocos2d::Component
{
public:
	typedef cocos2d::Vec2 Vector2;
	typedef std::string String;
	
	AttackComponent(float secondsBetweenAttacks);

	// Performs attack
	virtual void attack(const Vector2& direction);
	
	// Returns ready if owner is ready to attack
	virtual bool isReadyToAttack() const;

private:
	float m_secondsBetweenAttacks;
	long long m_lastTimeAttacked;
};