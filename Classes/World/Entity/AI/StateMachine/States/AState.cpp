#include "AState.h"

NS_LIGHTSOULS_BEGIN

AIState AState::GetNextStateOnSuccess() const
{
	return m_nextStateOnSuccess;
}

AIState AState::GetNextStateOnFailure() const
{
	return m_nextStateOnFailure;
}

AIState AState::GetStateFromString(String stateType)
{
	AIState state = AIState::NONE;
	if (stateType == "Chase")
	{
		state = AIState::CHASE;
	}
	else if (stateType == "Attack")
	{
		state = AIState::ATTACK;
	}
	else if (stateType == "Patrol")
	{
		state = AIState::PATROL;
	}
	else if (stateType == "Idle")
	{
		state = AIState::IDLE;
	}
	else if (stateType == "Signaling")
	{
		state = AIState::SIGNALING;
	}
	else
	{
		state = AIState::NONE;
	}

	return state;
}

void AState::SetNextStateOnFailure(const AIState& state)
{
	m_nextStateOnFailure = state;
}

void AState::SetNextStateOnSuccess(const AIState& state)
{
	m_nextStateOnSuccess = state;
}

NS_LIGHTSOULS_END