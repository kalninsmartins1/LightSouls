#include "AttackComponent.h"
#include "Utils/Utils.h"

AttackComponent::AttackComponent(float secondsBetweenAttacks):
	m_secondsBetweenAttacks(secondsBetweenAttacks),
	m_lastTimeAttacked(0)
{
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
