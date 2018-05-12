#include "AttackComponent.h"
#include "Utils/Utils.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

AttackComponent::AttackComponent(float secondsBetweenAttacks, float attackRange)
	: m_ownerEntity(nullptr)
	, m_lastTimeAttacked(0.0f)
	, m_secondsBetweenAttacks(secondsBetweenAttacks)
	, m_attackRange(attackRange)
	, m_staminaConsumption(0.0f)
{
}

float AttackComponent::GetAttackRange() const
{
	return m_attackRange;
}

float AttackComponent::GetStaminaConsumption() const
{
	return m_staminaConsumption;
}

void AttackComponent::Attack(const Vector2& direction)
{
	m_lastTimeAttacked = Utils::GetTimeStampInMilliseconds();
	m_ownerEntity->ConsumeStamina(m_staminaConsumption);
}

bool AttackComponent::IsReadyToAttack() const
{
	// Check if attack cooldown has passed
	const long long millisecondsSinceLastAttack =
		Utils::GetTimeStampInMilliseconds() - m_lastTimeAttacked;
	const double secondsSinceLastAttack =
		Utils::ConvertMillisecondsToSeconds(millisecondsSinceLastAttack);

	return secondsSinceLastAttack > m_secondsBetweenAttacks && 
		m_ownerEntity->HasEnoughtStamina(m_staminaConsumption) &&
		m_ownerEntity->IsReadyToAttack();
}

const Entity* LightSouls::AttackComponent::GetOwnerEntity() const
{
	return m_ownerEntity;
}

void AttackComponent::SetStaminaConsumption(float staminaConsumption)
{
	m_staminaConsumption = staminaConsumption;
}

void AttackComponent::setOwner(cocos2d::Node* owner)
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

NS_LIGHTSOULS_END


