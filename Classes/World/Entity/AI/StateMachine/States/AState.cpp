#include "AState.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

std::map<LightSouls::EAIState, LightSouls::String> LightSouls::AState::s_stateToString =
{
	{EAIState::ATTACK,		AI_STATE_ATTACK		},
	{EAIState::CHASE,		AI_STATE_CHASE		},
	{EAIState::IDLE,		AI_STATE_IDLE		},
	{EAIState::LINE_ATTACK, AI_STATE_LINE_ATTACK},
	{EAIState::PATROL,		AI_STATE_PATROL		},
	{EAIState::PAUSE,		AI_STATE_PAUSE		},
	{EAIState::SIGNALING,	AI_STATE_SIGNALING	},
};

EAIState AState::GetNextStateOnSuccess() const
{
	return m_nextStateOnSuccess;
}

EAIState AState::GetNextStateOnFailure() const
{
	return m_nextStateOnFailure;
}

EAIState AState::GetStateFromString(String stateTypeStr)
{
	EAIState state = EAIState::NONE;
	Utils::FindKeyByValue(s_stateToString, stateTypeStr, state);

	return state;
}

void AState::SetNextStateOnFailure(const EAIState& state)
{
	m_nextStateOnFailure = state;
}

void AState::SetNextStateOnSuccess(const EAIState& state)
{
	m_nextStateOnSuccess = state;
}

NS_LIGHTSOULS_END