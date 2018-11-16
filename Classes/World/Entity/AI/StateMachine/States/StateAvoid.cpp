#include "StateAvoid.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Entity/Entity.h"
#include "World/Entity/Components/Attack/GenericAttackComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "World/World.h"
#include "Events/OnCollisionBeginEventData.h"
#include "World/Entity/Components/AnimComponent.h"
#include "Utils/Utils.h"
#include "Utils/XML/XMLConsts.h"

StateAvoid::StateAvoid(AIAgent& agent)
	: AState(agent)
	, m_curProgress(EStateProgress::NONE)
	, m_attackComponent(nullptr)
	, m_startAvoidDistance(200.0f)
	, m_stopAvoidDistance(450.0f)
	, m_failDistance(560.0f)	
	, m_randomTime(0.0f)
	, m_isRandomTimeExpired(false)
{

}

EAIState StateAvoid::GetStateType() const
{
	return EAIState::AVOID;
}

void StateAvoid::OnEnter(AnimComponent* animComponent)
{	
	m_animComponent = animComponent;	
	m_curProgress = EStateProgress::IN_PROGRESS;	
	AIAgent& agent = GetAgent();
	m_attackComponent = agent.GetAttackComponent();
	m_isRandomTimeExpired = false;
	m_randomTime = Utils::GetRandValueWithinRange(0, 7.0f);
	Utils::StartTimerWithCallback(&agent, CC_CALLBACK_0(StateAvoid::OnRandomTimeExpired, this),
		m_randomTime);
	m_randomTargetOffset = Utils::GetRandomPositionWithinCircle(Vector2(0.0f, 0.0f), 150.0f);
}

EStateProgress StateAvoid::OnStep()
{
	AIAgent& agent = GetAgent();
	Entity* targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();

	const Vector2& targetPosition = targetEntity->getPosition() + m_randomTargetOffset;
	Vector2 toTarget = targetPosition - agent.getPosition();
	Vector2 toTargetNormalized = toTarget.getNormalized();
	float distanceSqrToTarget = toTarget.lengthSquared();

	if (m_attackComponent->IsReadyToAttack() && distanceSqrToTarget <= m_attackComponent->GetAttackRangeSqr() && m_isRandomTimeExpired)
	{
		m_curProgress = EStateProgress::DONE;			
	}
	else if (distanceSqrToTarget < m_startAvoidDistance * m_startAvoidDistance)
	{
		agent.SetMoveDirection(toTargetNormalized * -1);						
	}	
	else if(distanceSqrToTarget >= m_stopAvoidDistance * m_stopAvoidDistance)
	{		
		agent.SetMoveDirection(Vector2::ZERO);		
	}

	if (m_failDistance > 0 && distanceSqrToTarget > m_failDistance * m_failDistance)
	{
		m_curProgress = EStateProgress::FAILED;
	}

	ProcessAnimations();

	return m_curProgress;
}

void StateAvoid::OnExit()
{
	GetAgent().SetMoveDirection(Vector2::ZERO);
}

void StateAvoid::OnRandomTimeExpired()
{
	m_isRandomTimeExpired = true;
}

void StateAvoid::ProcessAnimations()
{
	AIAgent& agent = GetAgent();
	if (!agent.IsProcessing())
	{
		bool isMoving = agent.GetHeading().lengthSquared() > 0;
		if (isMoving && !m_animComponent->IsCurrrentlyPlayingAnim(GameConsts::ANIM_TYPE_RUN))
		{
			float curScaleX = agent.getScaleX();
			if (curScaleX > 0)
			{
				m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_RUN);
			}
			else
			{
				m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_RUN, true);
			}			
		}
		else if(!isMoving && !m_animComponent->IsCurrrentlyPlayingAnim(GameConsts::ANIM_TYPE_IDLE))
		{
			m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_IDLE);			
		}
	}
}

void StateAvoid::OnEventReceived(const String& receivedEvent, const AEventData& eventData)
{
	if (PhysicsManager::GetEventOnCollisionBegin() == receivedEvent)
	{
		const OnCollisionBeginEventData& collisionData = static_cast<const OnCollisionBeginEventData&>(eventData);
		if (collisionData.GetCollidedWithName() == World::GetNodeName())
		{
			const Vector2& collisionPoint = collisionData.GetCollisionPoint();
			AIAgent& agent = GetAgent();
			Vector2 awayFromCollision = agent.getPosition() - collisionPoint;
			agent.SetMoveDirection(awayFromCollision.getNormalized());
		}
	}
}

void StateAvoid::LoadXMLData(const XMLElement* xmlElement)
{
	AState::LoadXMLData(xmlElement);
	m_stopAvoidDistance = xmlElement->FloatAttribute(XMLConsts::STOP_AVOID_DISTANCE_ATTR);
	m_startAvoidDistance = xmlElement->FloatAttribute(XMLConsts::START_AVOID_DISTANCE_ATTR);
	m_failDistance = xmlElement->FloatAttribute(XMLConsts::AI_FAIL_DISTANCE);
}

