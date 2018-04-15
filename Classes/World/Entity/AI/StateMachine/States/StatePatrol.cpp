#include "StatePatrol.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "Utils/Utils.h"

StatePatrol::StatePatrol(AIAgent& agent)
	: m_agent(agent)
	, m_targetEntity(AIAgentManager::GetInstance()->GetTargetEntity())
	, m_curProgress(StateProgress::NONE)
	, m_animComponent(nullptr)
	, m_curTargetPosition(Vector2::ZERO)
	, m_isLookingAround(false)
{
}

void StatePatrol::OnEnter(AIAnimComponent* animComponent)
{
	m_animComponent = animComponent;
	m_curProgress = StateProgress::IN_PROGRESS;	
	GetRandomPositionInRange(m_curTargetPosition);
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
			m_agent.Move();
			m_animComponent->PlayRunAnimation();
			
			if (toTargetPosition.length() < 0.1f)
			{
				// Target position reached
				m_isLookingAround = true;

				// idle for specific time				
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
	using namespace cocos2d;

	// Check if target has been spotted
	const Vec2& agentPosition = m_agent.getPosition();
	const Vec2& targetEntityPosition = m_targetEntity.getPosition();
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
	using namespace cocos2d;

	m_animComponent->PlayIdleAnimation();
	auto delay = DelayTime::create(m_agent.GetPatrolPause());
	auto callback = CallFunc::create(CC_CALLBACK_0(StatePatrol::OnFinishedLookingAround, this));
	auto sequence = Sequence::create(delay, callback, nullptr);
	
	m_agent.runAction(sequence);
}

void StatePatrol::OnFinishedLookingAround()
{
	m_isLookingAround = false;
}
