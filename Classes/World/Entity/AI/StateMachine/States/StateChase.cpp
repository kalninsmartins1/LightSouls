#include "StateChase.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Entity/CustomActions/ActionSequence.h"
#include "World/Entity/Components/AnimComponent.h"
#include "GameConsts.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/Utils.h"
#include "cocos2d/external/tinyxml2/tinyxml2.h"

StateChase::StateChase(AIAgent& agent) 
	: AState(agent)	
	, m_curProgress(EStateProgress::NONE)	
	, m_targetEntity(nullptr)
	, m_chaseRadius(0.0f)
	, m_chaseStopDistance(0.0f)
	, m_randomTargetOffset(Vector2::GetZero())
{

}

void StateChase::OnEnter(AnimComponent& animComponent)
{
	m_targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	m_curProgress = EStateProgress::IN_PROGRESS;
	m_randomTargetOffset = Utils::GetRandomPositionWithinCircle(Vector2::GetZero(), 150.0f);
	m_animComponent = &animComponent;
}

EStateProgress StateChase::OnStep()
{
	AIAgent& agent = GetAgent();
	if (!agent.IsProcessing())
	{
		// Move agent towards target location
		const Vector2& currentPosition = agent.GetPos();
		const Vector2& targetPosition = m_targetEntity->GetPos() + m_randomTargetOffset;
		Vector2 toTarget = targetPosition - currentPosition;
		Vector2 toTargetNormalized = toTarget;
		toTargetNormalized.Normalize();

		// Update agent move direction
		agent.SetMoveDirection(toTargetNormalized);
		float distanceToTarget = toTarget.GetLenght();

		HandleAnimationPlaying(agent);

		if (distanceToTarget <= m_chaseStopDistance ||	// Target has been caught
			distanceToTarget >= m_chaseRadius)			// Target run off 
		{
			m_curProgress = EStateProgress::DONE;
		}
	}

	return m_curProgress;
}

void StateChase::OnExit()
{
	AIAgent& agent = GetAgent();
	m_curProgress = EStateProgress::NONE;
	agent.SetMoveDirection(Vector2::GetZero());

	if (!agent.IsProcessing())
	{
		m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_IDLE);
	}
}

void StateChase::LoadXMLData(const XMLElement& xmlElement)
{
	AState::LoadXMLData(xmlElement);
	m_chaseRadius = xmlElement.FloatAttribute(XMLConsts::AI_CHASE_RADIUS_ATTR);
	m_chaseStopDistance = xmlElement.FloatAttribute(XMLConsts::AI_STOP_DISTANCE);
}

void StateChase::HandleAnimationPlaying(const AIAgent& agent)
{
	// Play run animation if not playing, might be that on enter entity was processing
	if (!m_animComponent->IsCurrrentlyPlayingAnim(GameConsts::ANIM_TYPE_RUN))
	{
		float curScale = agent.getScaleX();
		if (curScale > 0)
		{
			m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_RUN);
		}
		else
		{
			m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_RUN, true);
		}
	}
}

EAIState StateChase::GetStateType() const
{
	return EAIState::CHASE;
}

