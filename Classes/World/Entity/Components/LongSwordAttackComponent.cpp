#include "LongSwordAttackComponent.h"

LongSwordAttackComponent* LongSwordAttackComponent::create(float secondsBetweenAttacks)
{
	LongSwordAttackComponent* pAttackComponent =
		new (std::nothrow) LongSwordAttackComponent(secondsBetweenAttacks);
	if (pAttackComponent != nullptr)
	{
		pAttackComponent->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pAttackComponent);
	}

	return pAttackComponent;
}

LongSwordAttackComponent::LongSwordAttackComponent(float secondsBetweenAttacks) :
	AttackComponent(secondsBetweenAttacks)
{
}

void LongSwordAttackComponent::attack(const Vector2& direction)
{
	AttackComponent::attack(direction);
}