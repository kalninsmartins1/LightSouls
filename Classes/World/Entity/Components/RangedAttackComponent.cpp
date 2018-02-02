#include "RangedAttackComponent.h"
#include "2d/CCNode.h"
#include "World/Entity/Ammo/Arrow.h"
#include "Utils/Utils.h"

RangedAttackComponent* RangedAttackComponent::create(
	const String& pathToAmmo,
	float maxAmmoFlyDistance,
	float ammoMoveSpeed,
	float secondsBetweenAttacks)
{
	RangedAttackComponent *component =
		new (std::nothrow) RangedAttackComponent(pathToAmmo,
			maxAmmoFlyDistance, ammoMoveSpeed, secondsBetweenAttacks);
	if (component != nullptr)
	{
		component->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(component);
	}

	return component;
}

RangedAttackComponent::RangedAttackComponent(const String& pathToAmmo,
	float maxAmmoFlyDistance, float ammoMoveSpeed,
	float secondsBetweenAttacks) :
	m_pathToAmmo(pathToAmmo),
	m_maxAmmoFlyDistance(maxAmmoFlyDistance),
	m_ammoMoveSpeed(ammoMoveSpeed),
	m_secondsBetweenAttacks(secondsBetweenAttacks),
	m_lastTimeAttacked(0)
{
}

void RangedAttackComponent::shoot(const Vector2& direction)
{	
	Arrow* pArrow = Arrow::create(m_pathToAmmo, cocos2d::Vec2(
		_owner->getPosition()),
		direction,
		m_maxAmmoFlyDistance,
		m_ammoMoveSpeed);
	
	// Spawn arrow in world
	cocos2d::Node* pParent = _owner->getParent();

	// Set the same camera mask to make arrow visible to camera
	pArrow->setCameraMask(pParent->getCameraMask());
	pParent->addChild(pArrow);

	m_lastTimeAttacked = Utils::getTimeStampInMilliseconds();	
}

bool RangedAttackComponent::isReadyToAttack() const
{
	const long millisecondsSinceLastAttack =
		Utils::getTimeStampInMilliseconds() - m_lastTimeAttacked;
	const float secondsSinceLastAttack =
		Utils::convertMillisecondsToSeconds(millisecondsSinceLastAttack);

	return secondsSinceLastAttack > m_secondsBetweenAttacks;
}