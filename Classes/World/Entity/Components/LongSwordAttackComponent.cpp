#include "LongSwordAttackComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "World/Entity/Entity.h"


LongSwordAttackComponent* LongSwordAttackComponent::create(float secondsBetweenAttacks, 
	float attackRange)
{
	LongSwordAttackComponent* pAttackComponent =
		new (std::nothrow) LongSwordAttackComponent(secondsBetweenAttacks, attackRange);
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

LongSwordAttackComponent::LongSwordAttackComponent(float secondsBetweenAttacks, float attackRange) :
	AttackComponent(secondsBetweenAttacks),
	m_attackRange(attackRange)	
{
}

bool LongSwordAttackComponent::onAttackHit(cocos2d::PhysicsWorld& world,
	cocos2d::PhysicsShape& shape, void* pMetaData) const
{
	CCLOG("LongSwordAttackComponent: Hit %s", shape.getBody()->getNode()->getName().c_str());
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

void LongSwordAttackComponent::checkAffectedObjects(const Vector2& direction) const
{
	// After attack finished check if we hit something
	const Entity* pEntity = dynamic_cast<Entity*>(_owner);
	const cocos2d::Size bodySize = pEntity->getPhysicsBodySize();
	const float bodyWidthScaled = bodySize.width * abs(pEntity->getScaleX());

	// Positin the querry rect right next to the owner collision rect
	const float paddingFromPlayer = 5.f;
	Vector2 rectOrgin = pEntity->getPosition() + direction * 
		(bodyWidthScaled + paddingFromPlayer);
	const float rectWidth = m_attackRange;
	const float rectHeight = m_attackRange * 2;
	
	CCLOG("Checking attacking !");
	// Move the rect anchor to middle
	rectOrgin -= Vector2(rectWidth / 2, rectHeight / 2);
	
	const cocos2d::Rect rect(rectOrgin,
		cocos2d::Size(rectWidth, rectHeight));
	PhysicsManager::getInstance()->debugDrawRect(rect);
	PhysicsManager::querryRect(rect, 
		CC_CALLBACK_3(LongSwordAttackComponent::onAttackHit,
			this));
}
