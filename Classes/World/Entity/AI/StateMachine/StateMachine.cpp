#include "StateMachine.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "Events/AEventData.h"
#include "Utils/Utils.h"
#include "States/Attack/StateAttack.h"
#include "States/Attack/StateLineAttack.h"
#include "States/StateChase.h"
#include "States/StatePatrol.h"
#include "States/StateIdle.h"
#include "States/StateSignaling.h"
#include "States/StatePause.h"
#include "States/StateAvoid.h"
#include "States/StateIsPlayerClose.h"
#include "World/Entity/CustomActions/AI/AIAvoidTargetAction.h"
#include "World/Physics/PhysicsManager.h"
#include "Events/OnCollisionBeginEventData.h"
#include "World/Entity/AI/AIAgentManager.h"

NS_LIGHTSOULS_BEGIN

StateMachine::StateMachine(AIAgent& agent)
	: m_agent(agent)
	, m_startState(EAIState::NONE)
	, m_curState(nullptr)
	, m_animComponent(nullptr)
	, m_availableStates()
{

}

EAIState StateMachine::GetCurrentState() const
{
	return m_curState->GetStateType();
}

void StateMachine::SetStartState(EAIState stateType)
{
	m_startState = stateType;
}

void StateMachine::Start(AnimComponent* animComponent)
{
	m_animComponent = animComponent;
	if (Utils::ContainsKey(m_availableStates, m_startState))
	{
		SwitchState(m_availableStates.at(m_startState));
	}
	else
	{
		CCASSERT(false, "StateMachine invalid start state!");
	}
}

void StateMachine::AddAvailableState(EAIState availableState, const XMLElement* xmlElement)
{
	AState* state = nullptr;
	switch (availableState)
	{
		case EAIState::CHASE:
			state = AState::Create<StateChase>(m_agent);
			break;

		case EAIState::ATTACK:
			state = AState::Create<StateAttack>(m_agent);
			break;

		case EAIState::LINE_ATTACK:
			state = AState::Create<StateLineAttack>(m_agent);
			break;

		case EAIState::PATROL:
			state = AState::Create<StatePatrol>(m_agent);
			break;

		case EAIState::IDLE:
			state = AState::Create<StateIdle>(m_agent);
			break;

		case EAIState::SIGNALING:			
			state = AState::Create<StateSignaling>(m_agent);			
			break;

		case EAIState::PAUSE:			
			state = AState::Create<StatePause>(m_agent);			
			break;

		case EAIState::AVOID:			
			state = AState::Create<StateAvoid>(m_agent);			
			break;

		case EAIState::IS_PLAYER_CLOSE:			
			state = AState::Create<StateIsPlayerClose>(m_agent);			
			break;
	}

	if (state != nullptr)
	{
		state->LoadXMLData(xmlElement);
		m_availableStates.insert(availableState, state);
	}
}

void StateMachine::Reset()
{
	SwitchState(m_startState);
}

void StateMachine::SwitchState(AState* newState)
{
	if (m_curState != nullptr)
	{
		m_curState->OnExit();
#if LIGHTSOULS_DEBUG_AI
		CCLOG("%s: OnExit!", AState::GetStringFromState(m_curState->GetStateType()).c_str());
#endif
	}

#if LIGHTSOULS_DEBUG_AI	
	CCLOG("%s: OnEnter!", AState::GetStringFromState(newState->GetStateType()).c_str());
#endif

	m_curState = newState;
	m_curState->OnEnter(m_animComponent);
}

void StateMachine::SwitchState(EAIState newState)
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
	const EStateProgress& curProgress = m_curState->OnStep();
	switch (curProgress)
	{
		case EStateProgress::IN_PROGRESS:
			// Wait for state to finish
			break;

		case EStateProgress::DONE:
			OnStateDone();
			break;

		case EStateProgress::FAILED:
			OnStateFailed();
			break;

		default:
			CCLOGERROR("StateMachine: [update] invalid state progress !");
			break;
	}
}

void StateMachine::DispatchEvent(const String& eventType, const AEventData& eventData)
{
	if (eventType == PhysicsManager::GetEventOnCollisionBegin()) 
	{
		Entity* targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
		const OnCollisionBeginEventData& collisionData = static_cast<const OnCollisionBeginEventData&>(eventData);
		if (collisionData.GetCollidedWithName() == targetEntity->getName())
		{
			//CCLOG("Collided with player !");
		}
	}

	m_curState->OnEventReceived(eventType, eventData);
}

void StateMachine::OnStateDone()
{
	EAIState nextState = m_curState->GetNextStateOnSuccess();
	if (nextState != EAIState::NONE)
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
	EAIState nextState = m_curState->GetNextStateOnFailure();
	if (nextState != EAIState::NONE)
	{
		SwitchState(nextState);
	}
	else
	{
		CCLOG("State has no next state on failure defined !");
	}
}

NS_LIGHTSOULS_END