#pragma once

#include "Classes/Core/Events/BaseEventData.h"
#include "Classes/External/CocosEngine.h"

class AnimComponent;
class AIAgent;
class String;

namespace tinyxml2
{
	class XMLElement;
};

using XMLElement = tinyxml2::XMLElement;


enum class EStateProgress
{
	NONE,
	IN_PROGRESS,
	DONE,
	FAILED,
};

enum class EAIState
{
	NONE,
	CHASE,
	ATTACK,
	LINE_ATTACK,
	PATROL,
	IDLE,
	SIGNALING,
	PAUSE,
	AVOID,
	IS_PLAYER_CLOSE
};


class AState : public cocos2d::Ref
{
public:
	AState(AIAgent& aiAgent);

public:	
	virtual EAIState	GetStateType() const = 0;
	EAIState			GetNextStateOnSuccess() const;
	EAIState			GetNextStateOnFailure() const;
	static EAIState		GetStateFromString(const String& stateType);
	static String		GetStringFromState(EAIState state);

	void			SetNextStateOnSuccess(const EAIState& state);
	void			SetNextStateOnFailure(const EAIState& state);

	// Called when state is first entered
	virtual void			OnEnter(AnimComponent& animComponent) = 0;

	// Called to progress the state, returns current state progress
	virtual EStateProgress	OnStep() = 0;

	// Called when state is exited
	virtual void			OnExit() = 0;

	virtual void			OnEventReceived(const String& receivedEvent, const BaseEventData& eventData);
	virtual void			LoadXMLData(const XMLElement& xmlElement);

	template<typename T>
	static T*				Create(AIAgent& agent);

protected:
	AIAgent& GetAgent() const;

private:
	static std::map<EAIState, String>	s_stateToString;
	EAIState							m_nextStateOnSuccess;
	EAIState							m_nextStateOnFailure;
	AIAgent&							m_agent;
};

template<typename T>
T* AState::Create(AIAgent& agent)
{
	T* state = new (std::nothrow) T(agent);

	if (state != nullptr)
	{
		state->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(state);
	}

	return state;
}


