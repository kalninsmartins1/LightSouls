#include "RangedAttackComponent.h"
#include "Utils/Utils.h"
#include "World/Entity/Entity.h"
#include "Classes/Core/Math/Vector2.h"
#include "World/Projectiles/Projectile.h"

RangedAttackComponent* RangedAttackComponent::Create(const ProjectileConfig& config, float range, float secondsBetweenAttacks)
{
	RangedAttackComponent *component = new (std::nothrow) RangedAttackComponent(config, range, secondsBetweenAttacks);
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

RangedAttackComponent::RangedAttackComponent(const ProjectileConfig& config, float attackRange, float secondsBetweenAttacks) 
	: GenericAttackComponent(secondsBetweenAttacks, attackRange)
	, m_projectileConfig(config)
{

}

void RangedAttackComponent::Attack(const Vector2& direction)
{	
	// Allow base class to perform its actions
	GenericAttackComponent::Attack(direction);

	const Entity* ownerEntity = GetOwnerEntity();
	Projectile* projectile = Projectile::Create(*ownerEntity, m_projectileConfig,
												direction, GetAttackRange());
	
	// Spawn arrow in world
	cc::Node* parent = _owner->getParent();

	// Set the same camera mask to make arrow visible to camera
	projectile->setCameraMask(parent->getCameraMask());
	parent->addChild(projectile);
}

