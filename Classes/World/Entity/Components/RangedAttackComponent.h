#pragma once

#include "2d/CCComponent.h"

typedef cocos2d::Vec2 Vector2;
typedef std::string String;

class RangedAttackComponent: public cocos2d::Component
{
public:
	static RangedAttackComponent* create(const String& pathToAmmo,
		float maxAmmoFlyDistance, float ammoMoveSpeed,
		float secondsBetweenAttacks);
	void shoot(const Vector2& direction);
	bool isReadyToAttack() const;

private:
	RangedAttackComponent(const String& pathToAmmo,
		float maxAmmoFlyDistance, float ammoMoveSpeed,
		float secondsBetweenAttacks);	

	String m_pathToAmmo;
	float m_maxAmmoFlyDistance;
	float m_ammoMoveSpeed;
	float m_secondsBetweenAttacks;
	long m_lastTimeAttacked;
};
