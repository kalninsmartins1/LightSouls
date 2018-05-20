#include "AttackComponent.h"
#include "Utils/Utils.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

AttackComponent::AttackComponent(float secondsBetweenAttacks, float attackRange)
	: m_ownerEntity(nullptr)
	, m_lastTimeAttacked(0.0f)
	, m_secondsBetweenAttacks(secondsBetweenAttacks)
	, m_comboExpireTime(0.0f)
	, m_attackRange(attackRange)
	, m_staminaConsumption(0.0f)
{
}

float AttackComponent::GetAttackRange() const
{
	return m_attackRange;
}

float LightSouls::AttackComponent::GetComboExpireTime() const
{
	return m_comboExpireTime;
}

float AttackComponent::GetStaminaConsumption() const
{
	return m_staminaConsumption;
}

bool LightSouls::AttackComponent::IsComboExpired() const
{
	return GetSecondsSinceLastAttack() > m_comboExpireTime;
}

void AttackComponent::Attack(const Vector2& direction)
{
	m_lastTimeAttacked = Utils::GetTimeStampInMilliseconds();
	m_ownerEntity->ConsumeStamina(m_staminaConsumption);
}

float LightSouls::AttackComponent::GetSecondsSinceLastAttack() const
{
	const long long millisecondsSinceLastAttack = Utils::GetTimeStampInMilliseconds() - m_lastTimeAttacked;	
	return Utils::ConvertMillisecondsToSeconds(millisecondsSinceLastAttack);
}

bool AttackComponent::IsReadyToAttack() const
{
	// Check if attack cooldown has passed
	bool isAttackCooledDown = GetSecondsSinceLastAttack() > m_secondsBetweenAttacks;
	bool hasEnoughStamina = m_ownerEntity->HasEnoughtStamina(m_staminaConsumption);
	bool isEntityReady = m_ownerEntity->IsReadyToAttack();

#if LIGHTSOULS_ATTACK_DEBUG
	CCLOG("AttackComponent entity %s | isAttackCooleddown %d | hasEnoughStamina %d | isEntityReady %d |", m_ownerEntity->getName().c_str(), isAttackCooledDown, hasEnoughStamina, isEntityReady);
#endif

	return isAttackCooledDown && hasEnoughStamina && isEntityReady;
}

const Entity* LightSouls::AttackComponent::GetOwnerEntity() const
{
	return m_ownerEntity;
}

void AttackComponent::SetComboExpireTime(float expireTime)
{
	m_comboExpireTime = expireTime;
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


