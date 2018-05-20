#include "LongSwordAttackComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

LongSwordAttackComponent* LongSwordAttackComponent::Create(float secondsBetweenAttacks,
	float attackRange, float paddingFromBody)
{
	LongSwordAttackComponent* pAttackComponent =
		new (std::nothrow) LongSwordAttackComponent(secondsBetweenAttacks, attackRange,
			paddingFromBody);
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

LongSwordAttackComponent::LongSwordAttackComponent(float secondsBetweenAttacks,
		float attackRange, float paddingFromBody) 
	: AttackComponent(secondsBetweenAttacks, attackRange)
	, m_paddingFromBody(paddingFromBody)
{
}

bool LongSwordAttackComponent::OnAttackHit(cocos2d::PhysicsWorld& world,
	cocos2d::PhysicsShape& shape, void* pMetaData) const
{
	Entity* hitEntity = dynamic_cast<Entity*>(shape.getBody()->getNode());

	// Ignore if hitting self
	if (hitEntity != nullptr && hitEntity->GetId() != GetOwnerEntity()->GetId())
	{
		hitEntity->TakeDamage(*GetOwnerEntity());
	}

	return true;
}

void LongSwordAttackComponent::Attack(const Vector2& direction)
{
	if (IsReadyToAttack())
	{
		AttackComponent::Attack(direction);
		CheckAffectedObjects(direction);
	}
}

void LongSwordAttackComponent::CheckAffectedObjects(const Vector2& direction) const
{
	// After attack finished check if we hit something
	const Entity* ownerEntity = GetOwnerEntity();
	const cocos2d::Size bodySize = ownerEntity->GetPhysicsBodySizeScaled();
	const float bodyWidthScaled = bodySize.width;

	Vector2 rectOrgin = ownerEntity->getPosition() + direction *
		(bodyWidthScaled + m_paddingFromBody);

	const float rectWidth = GetAttackRange();
	const float rectHeight = rectWidth * 2;
	
	// Move the rect anchor to middle
	rectOrgin -= Vector2(rectWidth / 2, rectHeight / 2);

	const cocos2d::Rect rect(rectOrgin,
		cocos2d::Size(rectWidth, rectHeight));

	PhysicsManager::QuerryRect(rect,
		CC_CALLBACK_3(LongSwordAttackComponent::OnAttackHit,
			this));
}

NS_LIGHTSOULS_END
