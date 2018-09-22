#include "LongSwordAttackComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

LongSwordAttackComponent* LongSwordAttackComponent::Create(float secondsBetweenAttacks,
	float attackRange, float paddingFromBody)
{
	LongSwordAttackComponent* attackComponent =
		new (std::nothrow) LongSwordAttackComponent(secondsBetweenAttacks, attackRange,
			paddingFromBody);
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

LongSwordAttackComponent::LongSwordAttackComponent(float secondsBetweenAttacks,
		float attackRange, float paddingFromBody) 
	: GenericAttackComponent(secondsBetweenAttacks, attackRange)
	, m_paddingFromBody(paddingFromBody)
{
}

bool LongSwordAttackComponent::OnAttackHit(cocos2d::PhysicsWorld& world,
	cocos2d::PhysicsShape& physicsObject, void* pMetaData) const
{
	TryToGiveDamage(physicsObject);

	return true;
}

void LongSwordAttackComponent::Attack(const Vector2& direction)
{
	if (IsReadyToAttack(GetOwnerEntity()->getPosition()))
	{
		GenericAttackComponent::Attack(direction);
		GenericAttackComponent::CheckAffectedObjects(*GetOwnerEntity(), *this, direction, m_paddingFromBody,
			CC_CALLBACK_3(LongSwordAttackComponent::OnAttackHit, this));
	}
}

NS_LIGHTSOULS_END
