#include "LongSwordAttackComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "World/Entity/Entity.h"


LongSwordAttackComponent* LongSwordAttackComponent::create(float secondsBetweenAttacks,
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
	float attackRange, float paddingFromBody) :
	AttackComponent(secondsBetweenAttacks),
	m_ownerEntity(nullptr),
	m_attackRange(attackRange),
	m_paddingFromBody(paddingFromBody)
{
}

bool LongSwordAttackComponent::OnAttackHit(cocos2d::PhysicsWorld& world,
	cocos2d::PhysicsShape& shape, void* pMetaData) const
{
	Entity* hitEntity = dynamic_cast<Entity*>(shape.getBody()->getNode());

	// Ignore if hitting self
	if (hitEntity != nullptr && hitEntity->GetId() != m_ownerEntity->GetId())
	{
		hitEntity->TakeDamage(m_ownerEntity->GetDamage());
		CCLOG("LongSwordAttackComponent: Hit %s",
			shape.getBody()->getNode()->getName().c_str());
	}

	return true;
}

void LongSwordAttackComponent::Attack(const Vector2& direction)
{
	if (AttackComponent::IsReadyToAttack())
	{
		AttackComponent::Attack(direction);
		CheckAffectedObjects(direction);
	}
}

void LongSwordAttackComponent::setOwner(cocos2d::Node* owner)
{
	if (owner != nullptr)
	{
		m_ownerEntity = dynamic_cast<Entity*>(owner);
		CCASSERT(m_ownerEntity != nullptr,
			"LongSwordAttackComponent: Owner is not an Entity !");
	}
	else
	{
		CCLOGERROR("LongSwordAttackComponent: Setting nullptr as owner !");
	}
}

void LongSwordAttackComponent::CheckAffectedObjects(const Vector2& direction) const
{
	// After attack finished check if we hit something	
	const cocos2d::Size bodySize = m_ownerEntity->GetPhysicsBodySize();
	const float bodyWidthScaled = bodySize.width * abs(m_ownerEntity->getScaleX());

	Vector2 rectOrgin = m_ownerEntity->getPosition() + direction *
		(bodyWidthScaled + m_paddingFromBody);
	const float rectWidth = m_attackRange;
	const float rectHeight = m_attackRange * 2;

	CCLOG("Checking attacking !");
	// Move the rect anchor to middle
	rectOrgin -= Vector2(rectWidth / 2, rectHeight / 2);

	const cocos2d::Rect rect(rectOrgin,
		cocos2d::Size(rectWidth, rectHeight));

	PhysicsManager::QuerryRect(rect,
		CC_CALLBACK_3(LongSwordAttackComponent::OnAttackHit,
			this));
}
