#include "HitAttackComponent.h"
#include "World/Entity/Entity.h"
#include "Classes/Core/Math/Vector2.h"


HitAttackComponent::HitAttackComponent(float secondsBetweenAttacks, float attackRange)
	: GenericAttackComponent(secondsBetweenAttacks, attackRange)
{

}

bool HitAttackComponent::OnAttackHit(cocos2d::PhysicsWorld& world, cocos2d::PhysicsShape& physicsObject, void* metaData) const
{
	TryToGiveDamage(physicsObject);
	return true;
}

HitAttackComponent* HitAttackComponent::Create(float secondsBetweenAttacks, float attackRange)
{
	HitAttackComponent* attackComponent = new (std::nothrow) HitAttackComponent(secondsBetweenAttacks, attackRange);
	if (attackComponent != nullptr)
	{
		attackComponent->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(attackComponent);
	}

	return attackComponent;
}

void HitAttackComponent::Attack(const Vector2& direction)
{
	if (IsReadyToAttack())
	{
		GenericAttackComponent::Attack(direction);
		GenericAttackComponent::CheckAffectedObjects(*GetOwnerEntity(), *this, direction, 10.0f,
			CC_CALLBACK_3(HitAttackComponent::OnAttackHit, this));
	}
}

void HitAttackComponent::RegisterSuccessfulAttack()
{
	GenericAttackComponent::Attack(Vector2::GetZero());
}

