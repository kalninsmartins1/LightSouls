#include "StatePatrol.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "Utils/Utils.h"

using namespace cocos2d;

StatePatrol::StatePatrol(AIAgent& agent):
	m_agent(agent),
	m_targetEntity(AIAgentManager::GetInstance()->getTargetEntity()),
	m_curProgress(StateProgress::NONE),
	m_pAnimComponent(nullptr)
{
}

void StatePatrol::onEnter(AIAnimComponent* pAnimComponent)
{
	m_pAnimComponent = pAnimComponent;
	m_curProgress = StateProgress::IN_PROGRESS;	
}

StateProgress StatePatrol::onStep()
{
	if(m_curProgress == StateProgress::IN_PROGRESS)
	{
		// First check if player has been spotted
		if (hasTargetBeenSpotted())
		{
			m_curProgress = StateProgress::DONE;
		}
		else
		{
			// If move action is fiished then start new move action
			const auto actionManager = m_agent.getActionManager();
			const auto moveToPositionAction = actionManager->
				getActionByTag(ACTION_MOVE_TAG, &m_agent);

			if (moveToPositionAction == nullptr ||
				moveToPositionAction->isDone())
			{
				moveToRandomPositionAndWait();
				m_pAnimComponent->playRunAnimation();
			}
		}		
	}

	return m_curProgress;
}

void StatePatrol::onExit()
{
	m_curProgress = StateProgress::NONE;
}

AIState StatePatrol::getStateType()
{
	return AIState::PATROL;
}

float StatePatrol::getTimeToReachTarget(const Vec2& targetPosition) const
{
	const Vec2& curAgentPosition = m_agent.getPosition();	
	const float distanceToTargetPosition = targetPosition.distance(curAgentPosition);
	const float agentMoveSpeed = m_agent.GetCurrentMoveSpeed();
	float timeToGetToTarget = 0;
	
	// Move speed should be positive number higher than 0
	if(agentMoveSpeed > 0)
	{
		timeToGetToTarget = distanceToTargetPosition / agentMoveSpeed;
	}

	return timeToGetToTarget;
}

bool StatePatrol::hasTargetBeenSpotted() const
{
	// Check if target has been spoted
	const Vec2& agentPosition = m_agent.getPosition();
	const Vec2& targetEntityPosition = m_targetEntity.getPosition();
	const float distanceToTargetEntity = targetEntityPosition
		.distance(agentPosition);

	return distanceToTargetEntity < m_agent.getAttackRadius();
}

void StatePatrol::moveToRandomPositionAndWait() const
{
	// Get random position within range
	const float angle = Utils::getRandAngleInRadians();
	const float agentActiveRadiues = m_agent.getActiveRadius();
	const float targetX = cos(angle) * agentActiveRadiues;
	const float targetY = sin(angle) * agentActiveRadiues;

	// Get time it takes for agent to move to position
	const Vec2 targetPosition = m_agent.getBasePosition() +
		Vec2(targetX, targetY);

	const float timeToReachTarget =
		getTimeToReachTarget(targetPosition);
	
	// Update agents moving direction
	const Vec2 toTarget = targetPosition - m_agent.getPosition();
	m_agent.SetMoveDirection(toTarget.getNormalized());

	// Move the agent to target position using move action
	const auto pMoveTo = MoveTo::create(timeToReachTarget, targetPosition);
	const auto pCallback = CallFunc::create(CC_CALLBACK_0(StatePatrol::onFinishedMoving, this));
	const auto pDelay = DelayTime::create(m_agent.getPatrolPause());
	Sequence* pSequence = Sequence::create(pMoveTo, pCallback, pDelay, nullptr);
	pSequence->setTag(ACTION_MOVE_TAG);
	m_agent.runAction(pSequence);
}

void StatePatrol::onFinishedMoving() const
{
	m_pAnimComponent->playIdleAnimation();
}
