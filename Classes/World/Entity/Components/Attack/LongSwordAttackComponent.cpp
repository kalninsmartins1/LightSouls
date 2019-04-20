#include "LongSwordAttackComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "World/Entity/Entity.h"
#include "Classes/Events/PositionEventData.h"
#include "Classes/GameConsts.h"
#include "Classes/Utils/Utils.h"

const String LongSwordAttackComponent::s_eventOnSlash = "EVENT_ON_ENTITY_SLASHED";

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

void LongSwordAttackComponent::SetDamageCheckDelay(const float damageCheckDelay)
{
	m_damageCheckDelay = damageCheckDelay;
}

LongSwordAttackComponent::LongSwordAttackComponent(float secondsBetweenAttacks,
		float attackRange, float paddingFromBody) 
	: GenericAttackComponent(secondsBetweenAttacks, attackRange)
	, m_paddingFromBody(paddingFromBody)
	, m_damageCheckDelay(0.0f)
	, m_lastAttackDirection(Vector2::ZERO)
{

}

bool LongSwordAttackComponent::OnAttackHit(cocos2d::PhysicsWorld& world,
	cocos2d::PhysicsShape& physicsObject, void* pMetaData) const
{
	TryToGiveDamage(physicsObject);

	return true;
}

void LongSwordAttackComponent::OnDamageCheck()
{
	GenericAttackComponent::CheckAffectedObjects(*GetOwnerEntity(), *this, m_lastAttackDirection, m_paddingFromBody,
		CC_CALLBACK_3(LongSwordAttackComponent::OnAttackHit, this));
}

void LongSwordAttackComponent::Attack(const Vector2& direction)
{
	if (IsReadyToAttack())
	{
		m_lastAttackDirection = direction;
		GenericAttackComponent::Attack(direction);
		Utils::StartTimerWithCallback(getOwner(),
			CC_CALLBACK_0(LongSwordAttackComponent::OnDamageCheck, this),
			m_damageCheckDelay, GameConsts::ACTION_ATTACK_CHECK_DAMAGE);
	}
}

void LongSwordAttackComponent::OnEntityHit(Entity* hitEntity) const
{
	const Entity* ownerEntity = GetOwnerEntity();
	if (ownerEntity != nullptr && hitEntity != nullptr)
	{
		const Vector2& ownerPosition = ownerEntity->getPosition();
		const Vector2& toHitEntity = hitEntity->getPosition() - ownerEntity->getPosition();
		const Vector2 hitPoint = ownerPosition + (toHitEntity.getNormalized() * GetAttackRange());
		ownerEntity->DispatchEvent(s_eventOnSlash, &PositionEventData(ownerEntity->GetId(), hitPoint));
	}
}