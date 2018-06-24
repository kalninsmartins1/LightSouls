#include "RangedAttackComponent.h"
#include "2d/CCNode.h"
#include "World/Entity/Ammo/Arrow.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

RangedAttackComponent* RangedAttackComponent::Create(
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
		float attackRange, float ammoMoveSpeed,
		float secondsBetweenAttacks) 
	: AttackComponent(secondsBetweenAttacks, attackRange)
	, m_pathToAmmo(pathToAmmo)
	, m_ammoMoveSpeed(ammoMoveSpeed)
{
}

void RangedAttackComponent::Attack(const Vector2& direction)
{	
	// Allow base class to perform its actions
	AttackComponent::Attack(direction);

	Arrow* pArrow = Arrow::Create(m_pathToAmmo, cocos2d::Vec2(
		_owner->getPosition()),
		direction,
		GetAttackRange(),
		m_ammoMoveSpeed);
	
	// Spawn arrow in world
	cocos2d::Node* pParent = _owner->getParent();

	// Set the same camera mask to make arrow visible to camera
	pArrow->setCameraMask(pParent->getCameraMask());
	pParent->addChild(pArrow);	
}

NS_LIGHTSOULS_END