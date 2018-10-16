#include "StateAvoid.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Entity/Entity.h"
#include "World/Entity/Components/Attack/GenericAttackComponent.h"
#include "World/Physics/PhysicsManager.h"
#include "World/World.h"
#include "Events/OnCollisionBeginEventData.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

StateAvoid::StateAvoid(AIAgent& agent)
	: AState(agent)
	, m_curProgress(EStateProgress::NONE)
	, m_attackComponent(nullptr)
	, m_startAvoidDistance(100.0f)
	, m_stopAvoidDistance(250.0f)
	, m_isAvoiding(false)
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

	if (!m_isAvoiding)
	{
		if (m_attackComponent->IsReadyToAttack(targetPosition) && m_isRandomTimeExpired)
		{
			m_curProgress = EStateProgress::DONE;			
		}
		else if (distanceSqrToTarget > m_attackComponent->GetAttackRangeSqr())
		{
			agent.SetMoveDirection(toTargetNormalized);
		}
		else if (distanceSqrToTarget < m_startAvoidDistance * m_startAvoidDistance)
		{
			agent.SetMoveDirection(toTargetNormalized * -1);
			m_isAvoiding = true;
		}
	}
	else if(distanceSqrToTarget >= m_stopAvoidDistance * m_stopAvoidDistance)
	{
		m_isAvoiding = false;
		agent.SetMoveDirection(Vector2::ZERO);
	}
	else if (m_isAvoiding && agent.GetHeading().lengthSquared() < 0.1f)
	{
		agent.SetMoveDirection(toTargetNormalized * -1);
	}

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

NS_LIGHTSOULS_END