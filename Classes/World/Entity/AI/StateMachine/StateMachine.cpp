#include "StateMachine.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "Events/AEventData.h"
#include "Utils/Utils.h"
#include "States/StateAttack.h"
#include "States/StateChase.h"
#include "States/StatePatrol.h"
#include "States/StateIdle.h"
#include "States/StateSignaling.h"

NS_LIGHTSOULS_BEGIN

StateMachine::StateMachine(AIAgent& agent) 
	: m_agent(agent)
	, m_startState(AIState::NONE)
	, m_curState(nullptr)
	, m_animComponent(nullptr)
	, m_availableStates()
{
}

void StateMachine::SetStartState(AIState stateType)
{
	m_startState = stateType;
}

void StateMachine::Start(AnimComponent* animComponent)
{
	m_animComponent = animComponent;
	if(Utils::ContainsKey(m_availableStates, m_startState))
	{
		SwitchState(m_availableStates.at(m_startState));
	}
	else
	{
		CCASSERT(false,"StateMachine invalid start state!");
	}
}

void StateMachine::AddAvailableState(AIState availableState, AIState stateOnSuccess, AIState stateOnFailure, float timeRestriction)
{
	AState* state = nullptr;
	switch (availableState)
	{
	case AIState::CHASE:
		state = AState::Create<StateChase>(m_agent);
		break;

	case AIState::ATTACK:
		state = AState::Create<StateAttack>(m_agent);
		break;

	case AIState::PATROL:
		state = AState::Create<StatePatrol>(m_agent);		
		break;

	case AIState::IDLE:
		state = AState::Create<StateIdle>(m_agent);
		break;

	case AIState::SIGNALING:
		state = AState::Create<StateSignaling>(m_agent);
		auto signaling = static_cast<StateSignaling*>(state);
		if (signaling != nullptr)
		{
			signaling->SetSignalingTime(timeRestriction);
		}
		break;
	}

	if (state != nullptr)
	{
		state->SetNextStateOnFailure(stateOnFailure);
		state->SetNextStateOnSuccess(stateOnSuccess);
		m_availableStates.insert(availableState, state);
	}
}

void StateMachine::SwitchState(AState* newState)
{	
	if(m_curState != nullptr)
	{
		m_curState->OnExit();
	}	
	m_curState = newState;
	m_curState->OnEnter(m_animComponent);	
}

void StateMachine::SwitchState(AIState newState)
{	
	if (Utils::ContainsKey(m_availableStates, newState))
	{
		SwitchState(m_availableStates.at(newState));
	}
	else
	{
		CCASSERT(false, "State is not available !");
	}
}

void StateMachine::OnStep()
{
	const StateProgress& curProgress = m_curState->OnStep();
	switch (curProgress)
	{
	case StateProgress::IN_PROGRESS:
		// Wait for state to finish		
		break;

	case StateProgress::DONE:
		OnStateDone();
			break;

	case StateProgress::FAILED:
		OnStateFailed();
		break;

	default:
		CCLOGERROR("StateMachine: [update] invalid state progress !");
		break;
	}
}

void StateMachine::DispatchEvent(const String& eventType, const AEventData& eventData)
{
	m_curState->OnEventReceived(eventType, eventData);
}

void StateMachine::OnStateDone()
{
	AIState nextState = m_curState->GetNextStateOnSuccess();
	if (nextState != AIState::NONE)
	{
		SwitchState(nextState);
	}
	else
	{
		CCLOG("State has no next state on success defined !");
	}
}

void StateMachine::OnStateFailed()
{	
	AIState nextState = m_curState->GetNextStateOnFailure();
	if (nextState != AIState::NONE)
	{
		SwitchState(nextState);
	}
	else
	{
		CCLOG("State has no next state on failure defined !");
	}
}

NS_LIGHTSOULS_END