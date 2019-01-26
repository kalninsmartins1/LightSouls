#include "StatePause.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"
#include "World/Entity/CustomActions/AI/AIAvoidTargetAction.h"


StatePause::StatePause(AIAgent& aiAgent)
	: AState(aiAgent)	
	, m_targetEntity(nullptr)
	, m_avoidAction(nullptr)
	, m_curProgress(EStateProgress::IN_PROGRESS)
	, m_pauseTime(0.0f)
	, m_shouldCheckDistanceToTarget(false)
{

}

void StatePause::OnEnter(AnimComponent& animComponent)
{
	AIAgent& agent = GetAgent();	
	if (!agent.IsProcessing() && agent.IsBackgroundTaskReady())
	{
		animComponent.PlayLoopingAnimation(GameConsts::ANIM_TYPE_IDLE);
	}

	m_curProgress = EStateProgress::IN_PROGRESS;
	m_targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	
	float pauseTime = m_pauseTime + Utils::GetRandValueWithinRange(0.0f, 5.0f);
	Utils::StartTimerWithCallback(&GetAgent(),
			CC_CALLBACK_0(StatePause::OnPauseExpired, this), pauseTime, GameConsts::ACTION_PAUSE);

}

EStateProgress StatePause::OnStep()
{	
	if (m_shouldCheckDistanceToTarget && IsTargetTooFar())
	{
		m_curProgress = EStateProgress::FAILED;
	}

	AIAgent& agent = GetAgent();
	if (!agent.IsProcessing())
	{
		agent.SetMoveDirection(Vector2::ZERO);
	}

	return m_curProgress;
}

void StatePause::OnExit()
{
	Utils::ClearCallbackTimers(&GetAgent(), GameConsts::ACTION_PAUSE);
}

void StatePause::LoadXMLData(const XMLElement& xmlElement)
{
	AState::LoadXMLData(xmlElement);
	m_pauseTime = xmlElement.FloatAttribute(XMLConsts::TIME_ATTR);
	m_maxDistaneceToTarget = xmlElement.FloatAttribute(XMLConsts::AI_MAX_DISTANCE_TO_TARGET);
	m_shouldCheckDistanceToTarget = xmlElement.BoolAttribute(XMLConsts::AI_IS_CHECKING_DISTANCE_TO_TARGET);
}

bool StatePause::IsTargetTooFar() const
{
	Entity* targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	Vector2 toTarget = targetEntity->getPosition() - GetAgent().getPosition();
	return  toTarget.lengthSquared() >= m_maxDistaneceToTarget * m_maxDistaneceToTarget;
}

void StatePause::OnPauseExpired()
{
	m_curProgress = EStateProgress::DONE;
}

EAIState StatePause::GetStateType() const
{
	return EAIState::PAUSE;
}

