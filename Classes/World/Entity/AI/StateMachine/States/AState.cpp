#include "AState.h"
#include "Utils/Utils.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"

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
	{EAIState::AVOID,		AI_STATE_AVOID		}
};

AState::AState(AIAgent& aiAgent)
	: m_agent(aiAgent)
{

}

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

String AState::GetStringFromState(EAIState state)
{
	String stateStr = "";
	if (Utils::ContainsKey(s_stateToString, state))
	{
		stateStr = s_stateToString[state];
	}

	return stateStr;
}

void AState::SetNextStateOnFailure(const EAIState& state)
{
	m_nextStateOnFailure = state;
}

void AState::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{

}

void AState::LoadXMLData(const XMLElement* xmlElement)
{
	String nextSuccessType;
	XMLLoader::ReadXMLAttribute(xmlElement, XML_AI_NEXT_STATE_ON_SUCCESS, nextSuccessType);
	String nextFailureType;
	XMLLoader::ReadXMLAttribute(xmlElement, XML_AI_NEXT_STATE_ON_FAILURE, nextFailureType);

	m_nextStateOnSuccess = AState::GetStateFromString(nextSuccessType);
	m_nextStateOnFailure = AState::GetStateFromString(nextFailureType);
}

AIAgent& AState::GetAgent() const
{
	return m_agent;
}

void AState::SetNextStateOnSuccess(const EAIState& state)
{
	m_nextStateOnSuccess = state;
}

NS_LIGHTSOULS_END