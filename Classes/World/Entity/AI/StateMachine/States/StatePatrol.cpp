#include "StatePatrol.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
#include "World/Entity/Components/AnimComponent.h"

NS_LIGHTSOULS_BEGIN

StatePatrol::StatePatrol(AIAgent& agent)
	: m_agent(agent)
	, m_targetEntity(AIAgentManager::GetInstance()->GetTargetEntity())
	, m_curProgress(StateProgress::NONE)
	, m_animComponent(nullptr)
	, m_curTargetPosition(Vector2::ZERO)
	, m_isLookingAround(false)
{
}

void StatePatrol::OnEnter(AnimComponent* animComponent)
{
	m_animComponent = animComponent;
	m_curProgress = StateProgress::IN_PROGRESS;	
	StartMovingToNewPosition();
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
			if (toTargetPosition.length() <= stoppingDistance || m_agent.IsCollided())
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

	auto delay = cocos2d::DelayTime::create(m_agent.GetPatrolPause());
	auto callback = cocos2d::CallFunc::create(CC_CALLBACK_0(StatePatrol::OnFinishedLookingAround, this));
	auto sequence = cocos2d::Sequence::create(delay, callback, nullptr);
	
	m_agent.runAction(sequence);
}

void StatePatrol::StartMovingToNewPosition()
{
	GetRandomPositionInRange(m_curTargetPosition);
	m_animComponent->PlayLoopingAnimation(ANIM_TYPE_RUN);
}

void StatePatrol::OnFinishedLookingAround()
{
	m_isLookingAround = false;
	StartMovingToNewPosition();
}

NS_LIGHTSOULS_END