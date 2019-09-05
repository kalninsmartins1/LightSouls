#pragma once

#include "Classes/External/CocosEngine.h"

namespace tinyxml2
{
	class XMLElement;
};

class AnimComponent;
class BaseEventData;
class AIAgent;
class AState;
class String;
enum class EAIState;

using XMLElement = tinyxml2::XMLElement;


class StateMachine
{
public:		
	StateMachine(AIAgent& agent);

public:
	EAIState GetCurrentState() const;

	void SetStartState(EAIState stateType);

	// Should be called once to turn on the state machine
	void Start(AnimComponent& animComponent);
	void AddAvailableState(const EAIState& availableState, const XMLElement& xmlElement);
	void Reset();

	// Should be called periodically to update current state of state machine 
	void OnStep();
	void DispatchEvent(const String& eventType, const BaseEventData& eventData);

private:
	void SwitchState(AState* newState);
	void SwitchState(EAIState newState);
	void OnStateDone();
	void OnStateFailed();

private:
	AIAgent&									m_agent;
	EAIState									m_startState;
	AState*										m_curState;
	AnimComponent*								m_animComponent;
	cc::Map<EAIState, AState*>					m_availableStates;
};


