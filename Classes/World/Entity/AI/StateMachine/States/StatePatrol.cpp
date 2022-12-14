#include "StatePatrol.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "Classes/Core/Events/OnCollisionBeginEventData.h"
#include "World/World.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"
#include "Classes/Utils/XML/XMLTypes.h"

StatePatrol::StatePatrol(AIAgent& agent)
	: AState(agent)
	, m_targetEntity(nullptr)
	, m_curProgress(EStateProgress::NONE)
	, m_animComponent(nullptr)
	, m_curTargetPosition(Vector2::GetZero())
	, m_isLookingAround(false)
	, m_isCollided(false)
	, m_patrolRadius(0.0f)
	, m_patrolPause(0.0f)
{

}

void StatePatrol::OnEnter(AnimComponent& animComponent)
{
	m_targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	m_animComponent = &animComponent;
	m_curProgress = EStateProgress::IN_PROGRESS;

	if (!GetAgent().IsProcessing())
	{
		StartMovingToNewPosition();
	}
}

EStateProgress StatePatrol::OnStep()
{
	if(m_curProgress == EStateProgress::IN_PROGRESS)
	{
		AIAgent& agent = GetAgent();

		// First check if player has been spotted
		if (HasTargetBeenSpotted())
		{
			m_curProgress = EStateProgress::DONE;
		}
		else if(!m_isLookingAround && !agent.IsProcessing())
		{
			Vector2 toTargetPosition = m_curTargetPosition - agent.GetPos();
			agent.SetMoveDirection(toTargetPosition.GetNormalized());

			if (!m_animComponent->IsCurrrentlyPlayingAnim(GameConsts::ANIM_TYPE_RUN))
			{
				m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_RUN);
			}
			
			float stoppingDistance = agent.GetStoppingDistance();
			if (toTargetPosition.GetLenght() <= stoppingDistance || m_isCollided)
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
	m_curProgress = EStateProgress::NONE;
	m_isLookingAround = false;

	// Clear any looking around timers
	GetAgent().stopAllActionsByTag(GameConsts::ACTION_TIMER);
}

void StatePatrol::OnEventReceived(const String& receivedEvent, const BaseEventData& eventData)
{
	if (receivedEvent == PhysicsManager::GetEventOnCollisionBegin())
	{
		const OnCollisionBeginEventData& collisionData = static_cast<const OnCollisionBeginEventData&>(eventData);
		if (collisionData.GetCollidedWithName() == World::GetNodeName())
		{
			m_isCollided = true;
		}
	}
}

void StatePatrol::LoadXMLData(const XMLElement& xmlElement)
{
	AState::LoadXMLData(xmlElement);
	m_patrolRadius = xmlElement.FloatAttribute(XMLConsts::AI_PATROL_RADIUS_ATTR);
	m_patrolPause = xmlElement.FloatAttribute(XMLConsts::AI_PATROL_PAUSE_ATTR);
}

EAIState StatePatrol::GetStateType() const
{
	return EAIState::PATROL;
}

bool StatePatrol::HasTargetBeenSpotted() const
{		
	// Check if target has been spotted
	const Vector2& agentPosition = GetAgent().GetPos();
	const Vector2& targetEntityPosition = m_targetEntity->GetPos();
	const Vector2& toTargetEntity = targetEntityPosition - agentPosition;
	const float distanceToTargetEntity = toTargetEntity.GetLenght();

	return distanceToTargetEntity < m_patrolRadius;
}

void StatePatrol::GetRandomPositionInRange(Vector2& outRandomPosition) const
{	
	outRandomPosition = Utils::GetRandomPositionWithinCircle(GetAgent().GetPos(),
		m_patrolRadius);	
}

void StatePatrol::StartLookingAround()
{	
	m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_IDLE);
	GetAgent().SetMoveDirection(Vector2::GetZero()); // We are not moving while looking around

	Utils::StartTimerWithCallback(&GetAgent(), 
		CC_CALLBACK_0(StatePatrol::OnFinishedLookingAround, this),
		m_patrolPause,
		GameConsts::ACTION_TIMER);
}

void StatePatrol::StartMovingToNewPosition()
{
	if(!m_isCollided)
	{
		GetRandomPositionInRange(m_curTargetPosition);
	}
	else
	{
		m_curTargetPosition = GetAgent().GetBasePosition();
		m_isCollided = false;
	}
	
	m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_RUN);
}

void StatePatrol::OnFinishedLookingAround()
{
	m_isLookingAround = false;
	StartMovingToNewPosition();
}