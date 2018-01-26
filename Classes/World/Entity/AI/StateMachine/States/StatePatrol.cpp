#include "StatePatrol.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "GameConsts.h"
#include "Utils/Utils.h"

using namespace cocos2d;

StatePatrol::StatePatrol(AIAgent& agent):
	m_agent(agent),
	m_targetEntity(AIAgentManager::getInstance()->getTargetEntity()),
	m_curProgress(StateProgress::NONE)
{
}

void StatePatrol::onEnter()
{
	m_curProgress = StateProgress::IN_PROGRESS;
}

StateProgress StatePatrol::onStep()
{
	if(m_curProgress == StateProgress::IN_PROGRESS)
	{
		// First check if player has been spotted
		if (hasPlayerBeenSpotted())
		{
			// State should be done and we should start doin other action
		}

		// If move action is fiished then start new move action
		const auto actionManager = m_agent.getActionManager();
		const auto moveToPositionAction = actionManager->
			getActionByTag(ACTION_MOVE_TAG, &m_agent);

		if(moveToPositionAction == nullptr ||
			moveToPositionAction->isDone())
		{
			moveToRandomPosition();
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
	const float agentMoveSpeed = m_agent.getCurrentMoveSpeed();
	float timeToGetToTarget = 0;
	
	// Move speed should be positive number higher than 0
	if(agentMoveSpeed > 0)
	{
		timeToGetToTarget = distanceToTargetPosition / agentMoveSpeed;
	}

	return timeToGetToTarget;
}

bool StatePatrol::hasPlayerBeenSpotted() const
{
	// Check if player has been spoted
	const Vec2& agentPosition = m_agent.getPosition();
	const Vec2& targetEntityPosition = m_targetEntity.getPosition();
	const float distanceToTargetEntity = targetEntityPosition
		.distance(agentPosition);

	return distanceToTargetEntity < m_agent.getActiveRadius();
}

void StatePatrol::moveToRandomPosition()
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

	// Move the agent to target position using move action
	const auto moveTo = MoveTo::create(timeToReachTarget, targetPosition);
	const auto delay = DelayTime::create(m_agent.getPatrolPause());
	Sequence* pSequence = Sequence::create(moveTo, delay, nullptr);
	pSequence->setTag(ACTION_MOVE_TAG);
	m_agent.runAction(pSequence);
}
