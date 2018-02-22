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
	m_pOwnerEntity(nullptr),
	m_attackRange(attackRange),
	m_paddingFromBody(paddingFromBody)
{
}

bool LongSwordAttackComponent::onAttackHit(cocos2d::PhysicsWorld& world,
	cocos2d::PhysicsShape& shape, void* pMetaData) const
{
	Entity* pHitEntity = dynamic_cast<Entity*>(shape.getBody()->getNode());

	// Ignore if hitting self
	if(pHitEntity->getId() != m_pOwnerEntity->getId())
	{
		pHitEntity->TakeDamage(m_pOwnerEntity->getDamage());
		CCLOG("LongSwordAttackComponent: Hit %s", 
			shape.getBody()->getNode()->getName().c_str());
	}
	
	return true;
}

void LongSwordAttackComponent::attack(const Vector2& direction)
{
	if(AttackComponent::isReadyToAttack())
	{
		AttackComponent::attack(direction);	
		checkAffectedObjects(direction);
	}
}

void LongSwordAttackComponent::setOwner(cocos2d::Node* owner)
{
	if(owner != nullptr)
	{
		m_pOwnerEntity = dynamic_cast<Entity*>(owner);
		CCASSERT(m_pOwnerEntity != nullptr,
			"LongSwordAttackComponent: Owner is not an Entity !");
	}
	else
	{
		CCLOGERROR("LongSwordAttackComponent: Setting nullptr as owner !");
	}	
}

void LongSwordAttackComponent::checkAffectedObjects(const Vector2& direction) const
{
	// After attack finished check if we hit something	
	const cocos2d::Size bodySize = m_pOwnerEntity->getPhysicsBodySize();
	const float bodyWidthScaled = bodySize.width * abs(m_pOwnerEntity->getScaleX());
	
	Vector2 rectOrgin = m_pOwnerEntity->getPosition() + direction * 
		(bodyWidthScaled + m_paddingFromBody);
	const float rectWidth = m_attackRange;
	const float rectHeight = m_attackRange * 2;
	
	CCLOG("Checking attacking !");
	// Move the rect anchor to middle
	rectOrgin -= Vector2(rectWidth / 2, rectHeight / 2);
	
	const cocos2d::Rect rect(rectOrgin,
		cocos2d::Size(rectWidth, rectHeight));

	PhysicsManager::querryRect(rect, 
		CC_CALLBACK_3(LongSwordAttackComponent::onAttackHit,
			this));
}
