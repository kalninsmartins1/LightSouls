#include "StateLineAttack.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Physics/PhysicsManager.h"
#include "World/Entity/Components/Attack/HitAttackComponent.h"
#include "World/Entity/Entity.h"
#include "Classes/Core/Events/OnCollisionBeginEventData.h"
#include "Utils/XML/XMLConsts.h"
#include "World/Entity/Components/AnimComponent.h"
#include "GameConsts.h"
#include "cocos2d/external/tinyxml2/tinyxml2.h"


StateLineAttack::StateLineAttack(AIAgent& aiAgent)	
	: AState(aiAgent)	
	, m_curProgress(EStateProgress::NONE)
	, m_targetEntity(nullptr)
	, m_targetPos(Vector2::GetZero())
	, m_attackComponent(nullptr)
	, m_moveSpeed(100.0f)
	, m_deliverDamageDistance(40.0f)
	, m_arriveDistance(40.0f)
{

}

EAIState StateLineAttack::GetStateType() const
{
	return EAIState::LINE_ATTACK;
}

void StateLineAttack::OnEnter(AnimComponent& animComponent)
{		
	AIAgent& agent = GetAgent();	
	m_attackComponent = agent.GetAttackComponent();	
	m_targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	animComponent.PlayLoopingAnimation(GameConsts::ANIM_TYPE_ATTACK, false);

	m_curProgress = EStateProgress::IN_PROGRESS;				
	SetTargetPosition(m_targetEntity->GetPos());
	agent.SetCurrentMoveSpeed(m_moveSpeed); // Move with different speed in this state	
}

EStateProgress StateLineAttack::OnStep()
{
	if (m_curProgress == EStateProgress::IN_PROGRESS)
	{
		AIAgent& agent = GetAgent();

		// Move to target
		const Vector2 agentPos = agent.GetPos();
		Vector2 toTarget = m_targetPos - agentPos;
		Vector2 toTargetNormalized = toTarget;
		toTargetNormalized.Normalize();
		agent.SetMoveDirection(toTargetNormalized);
		
		// Check if we are already there
		if (toTarget.GetLenghtSquared() <= m_arriveDistance * m_arriveDistance)
		{
			const Vector2& actualTargetPos = m_targetEntity->GetPos();
			Vector2 toActualPos = agentPos - actualTargetPos;
			if (toActualPos.GetLenghtSquared() < m_deliverDamageDistance * m_deliverDamageDistance)
			{
				OnSuccessfulAttack();
			}
			else
			{
				m_curProgress = EStateProgress::FAILED;
			}
		}
	}
		
	return m_curProgress;
}

void StateLineAttack::OnExit()
{
	m_attackComponent->Attack(Vector2::GetZero()); // Register attack was performed
	AIAgent& agent = GetAgent();
	agent.SetMoveDirection(Vector2::GetZero());
	agent.ResetMoveSpeed();
}

void StateLineAttack::OnEventReceived(const String& receivedEvent, const BaseEventData& eventData)
{
	if (receivedEvent == PhysicsManager::GetEventOnCollisionBegin())
	{
		auto colData = static_cast<const OnCollisionBeginEventData&>(eventData);
		if (colData.GetCollidedWithName() == m_targetEntity->getName())
		{
			OnSuccessfulAttack();
		}
	}
	else if (receivedEvent == AIAgent::GetEventOnHealthChanged())
	{
		m_curProgress = EStateProgress::FAILED;
	}
}

void StateLineAttack::LoadXMLData(const XMLElement& xmlElement)
{
	AState::LoadXMLData(xmlElement);
	m_moveSpeed = xmlElement.FloatAttribute(XMLConsts::MOVE_SPEED_ATTR);
	m_arriveDistance = xmlElement.FloatAttribute(XMLConsts::AI_ARRIVE_DISTANCE);
	m_deliverDamageDistance = xmlElement.FloatAttribute(XMLConsts::AI_DAMAGE_DISTANCE);
}

void StateLineAttack::OnSuccessfulAttack()
{
	m_curProgress = EStateProgress::DONE;	
	m_targetEntity->TakeDamage(GetAgent());
}

void StateLineAttack::SetTargetPosition(const Vector2& targetPos)
{
	AIAgent& agent = GetAgent();
	Vector2 agentPos = agent.GetPos();
	Vector2 toTarget = targetPos - agentPos;
	if (toTarget.GetLenghtSquared() > m_attackComponent->GetAttackRangeSqr())
	{
		m_targetPos = agentPos + toTarget.GetNormalized() * m_attackComponent->GetAttackRange();
	}
	else
	{
		m_targetPos = targetPos;
	}
}