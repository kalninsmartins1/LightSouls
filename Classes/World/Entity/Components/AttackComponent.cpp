#include "AttackComponent.h"
#include "Utils/Utils.h"

AttackComponent::AttackComponent(float secondsBetweenAttacks):
	m_secondsBetweenAttacks(secondsBetweenAttacks),
	m_lastTimeAttacked(0)
{
}

void AttackComponent::attack(const Vector2& direction)
{
	m_lastTimeAttacked = Utils::getTimeStampInMilliseconds();
}

bool AttackComponent::isReadyToAttack() const
{
	const long millisecondsSinceLastAttack =
		Utils::getTimeStampInMilliseconds() - m_lastTimeAttacked;
	const float secondsSinceLastAttack =
		Utils::convertMillisecondsToSeconds(millisecondsSinceLastAttack);

	return secondsSinceLastAttack > m_secondsBetweenAttacks;	
}
