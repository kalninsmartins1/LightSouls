#include "GenericAttackComponent.h"
#include "Utils/Utils.h"
#include "World/Entity/Entity.h"
#include "World/Physics/PhysicsManager.h"
#include "Scenes/GameScene.h"
#include "Classes/Core/Math/Vector2.h"

GenericAttackComponent* GenericAttackComponent::Create(float secondsBetweenAttacks, float attackRange)
{
	auto attackComponent = new (std::nothrow) GenericAttackComponent(secondsBetweenAttacks, attackRange);
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

GenericAttackComponent::GenericAttackComponent(float secondsBetweenAttacks, float attackRange)
	: m_ownerEntity(nullptr)
	, m_lastTimeAttacked(0.0f)
	, m_secondsBetweenAttacks(secondsBetweenAttacks)
	, m_comboExpireTime(0.0f)
	, m_attackRange(attackRange)	
{

}

float GenericAttackComponent::GetAttackRange() const
{
	return m_attackRange;
}

float GenericAttackComponent::GetAttackRangeSqr() const
{
	return m_attackRange * m_attackRange;
}

float GenericAttackComponent::GetComboExpireTime() const
{
	return m_comboExpireTime;
}

bool GenericAttackComponent::IsComboExpired() const
{
	return GetSecondsSinceLastAttack() > m_comboExpireTime;
}

void GenericAttackComponent::Attack(const Vector2& direction)
{
	m_lastTimeAttacked = Utils::GetTimeStampInMilliseconds();	
}

void GenericAttackComponent::CheckAffectedObjects(const Entity& attacker,
	const GenericAttackComponent& attackComponent, const Vector2& direction,
	float paddingFromBody, const QueryRectCallback& callback)
{
	// After attack finished check if we hit something	
	const cc::Size bodySize = attacker.GetPhysicsBodySizeScaled();
	const float bodyWidthScaled = bodySize.width;

	Vector2 rectOrgin = attacker.GetPos() + direction *
		(bodyWidthScaled + paddingFromBody);

	const float rectWidth = attackComponent.GetAttackRange();
	const float rectHeight = rectWidth * 2;

	// Move the rect anchor to middle
	rectOrgin = rectOrgin - Vector2(rectWidth / 2, rectHeight / 2);

	const cocos2d::Rect rect(cc::Vec2(rectOrgin.GetX(), rectOrgin.GetY()),
		cocos2d::Size(rectWidth, rectHeight));

	// Debug attack collision
	//GameScene::GetPhysicsManager()->DebugDrawRect(rect);

	PhysicsManager::QuerryRect(rect, callback);
}

void GenericAttackComponent::TryToGiveDamage(cc::PhysicsShape& physicsObject) const
{
	Entity* hitEntity = dynamic_cast<Entity*>(physicsObject.getBody()->getNode());
	const Entity* ownerEntity = GetOwnerEntity();
	
	if (hitEntity != nullptr &&
		hitEntity->GetId() != ownerEntity->GetId() && // Ignore if hitting self	
		typeid(hitEntity) != typeid(ownerEntity)) // Ignore hitting self kind
	{		
		OnEntityHit(hitEntity);
		hitEntity->TakeDamage(*ownerEntity);
	}
}

void GenericAttackComponent::OnEntityHit(Entity* hitEntity) const
{

}

float GenericAttackComponent::GetSecondsSinceLastAttack() const
{
	const long long millisecondsSinceLastAttack = Utils::GetTimeStampInMilliseconds() - m_lastTimeAttacked;	
	return Utils::ConvertMillisecondsToSeconds(millisecondsSinceLastAttack);
}

bool GenericAttackComponent::IsReadyToAttack() const
{
	// Check if attack cooldown has passed
	bool isAttackCooledDown = GetSecondsSinceLastAttack() > (m_secondsBetweenAttacks - Utils::GetRandValueWithinRange(0.0f, 2.0f));	
	bool isEntityReady = m_ownerEntity->IsReadyToAttack();

#if LIGHTSOULS_ATTACK_DEBUG
	CCLOG("AttackComponent entity %s | isAttackCooleddown %d | hasEnoughStamina %d | isEntityReady %d |", m_ownerEntity->getName().c_str(), isAttackCooledDown, hasEnoughStamina, isEntityReady);
#endif

	return isAttackCooledDown && isEntityReady;
}

const Entity* GenericAttackComponent::GetOwnerEntity() const
{
	return m_ownerEntity;
}

void GenericAttackComponent::SetComboExpireTime(float expireTime)
{
	m_comboExpireTime = expireTime;
}

void GenericAttackComponent::setOwner(cocos2d::Node* owner)
{
	// Call base class
	Component::setOwner(owner);

	if (owner != nullptr)
	{
		m_ownerEntity = dynamic_cast<Entity*>(owner);
		CCASSERT(m_ownerEntity != nullptr,
			"GenericAttackComponent: Owner is not an Entity !");
	}
	else
	{
		CCLOGERROR("GenericAttackComponent: Setting nullptr as owner !");
	}
}