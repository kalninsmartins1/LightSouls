#include "StatePatrol.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "Events/OnCollisionBeginEventData.h"
#include "World/World.h"

NS_LIGHTSOULS_BEGIN

StatePatrol::StatePatrol(AIAgent& agent)
	: m_agent(agent)
	, m_targetEntity(AIAgentManager::GetInstance()->GetTargetEntity())
	, m_curProgress(StateProgress::NONE)
	, m_animComponent(nullptr)
	, m_curTargetPosition(Vector2::ZERO)
	, m_isLookingAround(false)
	, m_isCollided(false)
{
}

void StatePatrol::OnEnter(AnimComponent* animComponent)
{
	m_animComponent = animComponent;
	m_curProgress = StateProgress::IN_PROGRESS;	
	StartMovingToNewPosition();
	CCLOG("Enter patrol state !");
}

StateProgress StatePatrol::OnStep()
{
	if(m_curProgress == StateProgress::IN_PROGRESS)
	{
		// First check if player has been spotted
		if (HasTargetBeenSpotted())
		{
			m_curProgress = StateProgress::DONE;
		}
		else if(!m_isLookingAround)
		{
			Vector2 toTargetPosition = m_curTargetPosition - m_agent.getPosition();
			m_agent.SetMoveDirection(toTargetPosition.getNormalized());
			
			float stoppingDistance = m_agent.GetStoppingDistance();
			if (toTargetPosition.length() <= stoppingDistance || m_isCollided)
			{
				// Target position reached
				m_isLookingAround = true;

				// Idle for specific time
				StartLookingAround();
			}
		}
	}

	return m_curProgress;
}

void StatePatrol::OnExit()
{
	m_curProgress = StateProgress::NONE;
	m_isLookingAround = false;

	// Clear any looking around timers
	m_agent.stopAllActionsByTag(ACTION_TIMER_TAG);
	CCLOG("Exit patrol state !");
}

void StatePatrol::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{
	if (receivedEvent == PhysicsManager::GetEventOnCollisionBegin())
	{
		const OnCollisionBeginEventData& collisionData = static_cast<const OnCollisionBeginEventData&>(eventData);
		if (collisionData.GetCollidedWithName() == World::GetNodeName())
		{
			CCLOG("State Patrol Collided !");
			m_isCollided = true;
		}
	}
}

AIState StatePatrol::GetStateType()
{
	return AIState::PATROL;
}

bool StatePatrol::HasTargetBeenSpotted() const
{		
	// Check if target has been spotted
	const Vector2& agentPosition = m_agent.getPosition();
	const Vector2& targetEntityPosition = m_targetEntity.getPosition();
	const float distanceToTargetEntity = targetEntityPosition
		.distance(agentPosition);

	return distanceToTargetEntity < m_agent.GetChaseRadius();
}

void StatePatrol::GetRandomPositionInRange(Vector2& outRandomPosition) const
{	
	outRandomPosition = Utils::GetRandomPositionWithinCircle(m_agent.getPosition(),
		m_agent.GetPatrolRadius());	
}

void StatePatrol::StartLookingAround()
{	
	m_animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
	m_agent.SetMoveDirection(Vector2::ZERO); // We are not moving while looking around

	Utils::StartTimerWithCallback(&m_agent, 
		CC_CALLBACK_0(StatePatrol::OnFinishedLookingAround, this),
		m_agent.GetPatrolPause(),
		ACTION_TIMER_TAG);
}

void StatePatrol::StartMovingToNewPosition()
{
	if(!m_isCollided)
	{
		GetRandomPositionInRange(m_curTargetPosition);
	}
	else
	{
		m_curTargetPosition = m_agent.GetBasePosition();
		m_isCollided = false;
	}
	
	m_animComponent->PlayLoopingAnimation(ANIM_TYPE_RUN);
}

void StatePatrol::OnFinishedLookingAround()
{
	m_isLookingAround = false;
	StartMovingToNewPosition();
}

NS_LIGHTSOULS_END