#include "LongSwordAttackComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "World/Entity/Entity.h"
#include "Classes/Core/Events/TransformEventData.h"
#include "Classes/GameConsts.h"
#include "Classes/Utils/Utils.h"

const char* LongSwordAttackComponent::s_eventOnSlash = "EVENT_ON_ENTITY_SLASHED";
const char* LongSwordAttackComponent::s_eventOnLongSwordAttackStarted = "EVENT_ON_LONG_SWORD_ATTACK_STARTED";
const char* LongSwordAttackComponent::s_hitOffsetPropName = "hitPointOffset";

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

const char* LongSwordAttackComponent::GetHitOffsetPropName()
{
	return s_hitOffsetPropName;
}

void LongSwordAttackComponent::SetDamageCheckDelay(const float damageCheckDelay)
{
	m_damageCheckDelay = damageCheckDelay;
}

void LongSwordAttackComponent::SetHitPointOffset(float hitPointOffset)
{
	m_hitPointOffset = hitPointOffset;
}

LongSwordAttackComponent::LongSwordAttackComponent(float secondsBetweenAttacks,
		float attackRange, float paddingFromBody) 
	: GenericAttackComponent(secondsBetweenAttacks, attackRange)
	, m_paddingFromBody(paddingFromBody)
	, m_damageCheckDelay(0.0f)
	, m_hitPointOffset(0.0f)
	, m_lastAttackDirection(Vector2::GetZero())
{

}

bool LongSwordAttackComponent::OnAttackHit(cocos2d::PhysicsWorld& world,
	cocos2d::PhysicsShape& physicsObject, void* metaData) const
{
	TryToGiveDamage(physicsObject);

	return true;
}

void LongSwordAttackComponent::OnDamageCheck()
{
	GenericAttackComponent::CheckAffectedObjects(*GetOwnerEntity(), *this, m_lastAttackDirection, m_paddingFromBody,
		CC_CALLBACK_3(LongSwordAttackComponent::OnAttackHit, this));
	
	if (m_attackFinishedCallback != nullptr)
	{
		m_attackFinishedCallback();
	}
}

void LongSwordAttackComponent::DispatchStartAttackEvent(const Vector2& direction)
{
	const Entity* ownerEntity = GetOwnerEntity();
	if (ownerEntity != nullptr)
	{
		float angle = Utils::GetSignedAngleBetweenVectors(direction, Vector2(1, 0));
		Vector2 position = ownerEntity->GetPos() + direction * GetAttackRange();
		TransformEventData transformData(ownerEntity->GetId(), position, angle);
		ownerEntity->DispatchEvent(s_eventOnLongSwordAttackStarted, &transformData);
	}
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

		DispatchStartAttackEvent(direction);
	}
}

void LongSwordAttackComponent::SetAttackFinishCallback(const AttackFinishedCallback & callback)
{
	m_attackFinishedCallback = callback;
}

void LongSwordAttackComponent::OnEntityHit(Entity* hitEntity) const
{
	const Entity* ownerEntity = GetOwnerEntity();
	if (ownerEntity != nullptr && hitEntity != nullptr)
	{
		const Vector2& ownerPosition = ownerEntity->GetPos();
		const Vector2& toHitEntity = (hitEntity->GetPos() - ownerEntity->GetPos()).GetNormalized();
		float distanceToHitPoint = GetAttackRange() + m_hitPointOffset;
		const Vector2 hitPoint = ownerPosition + (toHitEntity * distanceToHitPoint);

		TransformEventData transformData(ownerEntity->GetId(), hitPoint, 0.0f);
		ownerEntity->DispatchEvent(s_eventOnSlash, &transformData);
	}
}