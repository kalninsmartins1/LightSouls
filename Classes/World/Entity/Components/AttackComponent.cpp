#include "AttackComponent.h"
#include "Utils/Utils.h"

AttackComponent::AttackComponent(float secondsBetweenAttacks, float attackRange)
	: m_lastTimeAttacked(0)
	, m_secondsBetweenAttacks(secondsBetweenAttacks)
	, m_attackRange(attackRange)
{
}

float AttackComponent::GetAttackRange() const
{
	return m_attackRange;
}

void AttackComponent::Attack(const Vector2& direction)
{
	m_lastTimeAttacked = Utils::GetTimeStampInMilliseconds();
}

bool AttackComponent::IsReadyToAttack() const
{
	const long long millisecondsSinceLastAttack =
		Utils::GetTimeStampInMilliseconds() - m_lastTimeAttacked;
	const double secondsSinceLastAttack =
		Utils::ConvertMillisecondsToSeconds(millisecondsSinceLastAttack);

	return secondsSinceLastAttack > m_secondsBetweenAttacks;
}
