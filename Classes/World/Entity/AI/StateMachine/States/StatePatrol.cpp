#include "StatePatrol.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "Utils/Utils.h"

StatePatrol::StatePatrol(AIAgent& agent):
	m_agent(agent),
	m_targetEntity(AIAgentManager::GetInstance()->GetTargetEntity()),
	m_curProgress(StateProgress::NONE),
	m_animComponent(nullptr)
{
}

void StatePatrol::OnEnter(AIAnimComponent* animComponent)
{
	m_animComponent = animComponent;
	m_curProgress = StateProgress::IN_PROGRESS;	
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
		else
		{
			// If move action is finished then start new move action
			const auto actionManager = m_agent.getActionManager();
			const auto moveToPositionAction = actionManager->
				getActionByTag(ACTION_MOVE_TAG, &m_agent);

			if (moveToPositionAction == nullptr ||
				moveToPositionAction->isDone())
			{
				MoveToRandomPositionAndWait();
				m_animComponent->PlayRunAnimation();
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

float StatePatrol::GetTimeToReachTarget(const cocos2d::Vec2& targetPosition) const
{
	using namespace cocos2d;
	
	const Vec2& curAgentPosition = m_agent.getPosition();	
	const float distanceToTargetPosition = targetPosition.distance(curAgentPosition);
	const float agentMoveSpeed = m_agent.GetCurrentMoveSpeed();
	float timeToGetToTarget = 0;
	
	timeToGetToTarget = Utils::SafeDevide(distanceToTargetPosition, agentMoveSpeed);

	return timeToGetToTarget;
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

void StatePatrol::MoveToRandomPositionAndWait() const
{
	using namespace cocos2d;

	const Vec2& targetPosition = Utils::GetRandomPositionWithinCircle(m_agent.getPosition(),
		m_agent.GetPatrolRadius());

	// Get time it takes for agent to move to position
	const float timeToReachTarget = GetTimeToReachTarget(targetPosition);
	
	// Update agents moving direction
	const Vec2 toTarget = targetPosition - m_agent.getPosition();
	m_agent.SetMoveDirection(toTarget.getNormalized());

	// Move the agent to target position using move action
	const auto moveTo = MoveTo::create(timeToReachTarget, targetPosition);
	const auto callback = CallFunc::create(CC_CALLBACK_0(StatePatrol::OnFinishedMoving, this));
	const auto delay = DelayTime::create(m_agent.GetPatrolPause());
	Sequence* sequence = Sequence::create(moveTo, callback, delay, nullptr);
	sequence->setTag(ACTION_MOVE_TAG);
	m_agent.runAction(sequence);
}

void StatePatrol::OnFinishedMoving() const
{
	m_animComponent->PlayIdleAnimation();
}
