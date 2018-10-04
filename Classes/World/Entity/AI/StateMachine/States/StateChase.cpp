#include "StateChase.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Entity/CustomActions/ActionSequence.h"
#include "World/Entity/Components/AnimComponent.h"
#include "GameConsts.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"

NS_LIGHTSOULS_BEGIN

StateChase::StateChase(AIAgent& agent) 
	: AState(agent)	
	, m_curProgress(EStateProgress::NONE)	
	, m_targetEntity(nullptr)
	, m_chaseRadius(0.0f)
	, m_chaseStopDistance(0.0f)
{

}

void StateChase::OnEnter(AnimComponent* animComponent)
{
	m_targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	m_curProgress = EStateProgress::IN_PROGRESS;
	m_animComponent = animComponent;
}

EStateProgress StateChase::OnStep()
{
	AIAgent& agent = GetAgent();
	if (!agent.IsProcessing())
	{
		// Move agent towards target location
		const Vector2& currentPosition = agent.getPosition();
		Vector2 toTarget = m_targetEntity->getPosition() - currentPosition;
		const Vector2& toTargetNormalized = toTarget.getNormalized();

		// Update agent move direction
		agent.SetMoveDirection(toTargetNormalized);
		float distanceToTarget = toTarget.length();

		// Play run animation if not playing, might be that on enter entity was processing
		if (!m_animComponent->IsCurrrentlyPlayingAnim(ANIM_TYPE_RUN))
		{
			m_animComponent->PlayLoopingAnimation(ANIM_TYPE_RUN);
		}

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
	agent.SetMoveDirection(Vector2::ZERO);

	if (!agent.IsProcessing())
	{
		m_animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
	}
}

void StateChase::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{
	// ...
}

void StateChase::LoadXMLData(const XMLElement* xmlElement)
{
	AState::LoadXMLData(xmlElement);
	m_chaseRadius = xmlElement->FloatAttribute(XML_AI_CHASE_RADIUS_ATTR);
	m_chaseStopDistance = xmlElement->FloatAttribute(XML_AI_STOP_DISTANCE);
}

EAIState StateChase::GetStateType() const
{
	return EAIState::CHASE;
}

NS_LIGHTSOULS_END