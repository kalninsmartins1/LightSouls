#include "AIAvoidTargetAction.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/XML/XMLLoader.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Physics/PhysicsManager.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/StateMachine/States/AState.h"
#include "World/Entity/Components/AnimComponent.h"
#include "cocos2d/external/tinyxml2/tinyxml2.h"


AIAvoidTargetAction::AIAvoidTargetAction(AIAgent& agent)
	: m_agent(agent)
	, m_startAvoidingDistance(0.0f)
	, m_stopAvoidingDistance(0.0f)
	, m_isAvoiding(false)
	, m_isAvoidCooldownActive(false)
	, m_isAvoidTime(true)
	, m_activeTime(2.0f)
	, m_cooldownTime(1.0f)
{
	// Start avoid timer
	Utils::StartTimerWithCallback(&m_agent,
		CC_CALLBACK_0(AIAvoidTargetAction::OnAvoidTimerFinished, this),
		m_activeTime, GameConsts::ACTION_AI_AVOID);
}

AIAvoidTargetAction::~AIAvoidTargetAction()
{

}

bool AIAvoidTargetAction::IsTargetTooClose(const Entity* target) const
{
	float startDistSqr = m_startAvoidingDistance * m_startAvoidingDistance;
	float currDistSqr = m_agent.getPosition().distanceSquared(target->getPosition());

	return currDistSqr <= startDistSqr;
}

bool AIAvoidTargetAction::IsTargetFarEnough(const Entity* target) const
{
	float stopDistSqrt = m_stopAvoidingDistance * m_stopAvoidingDistance;
	float curDistSqrt = m_agent.getPosition().distanceSquared(target->getPosition());

	return curDistSqrt >= stopDistSqrt;
}

void AIAvoidTargetAction::OnCollisionCheck()
{
	// Keep checking for collision
	Utils::StartTimerWithCallback(&m_agent,
		CC_CALLBACK_0(AIAvoidTargetAction::OnCollisionCheck, this),
		m_collisionCheckInterval, GameConsts::ACTION_COLLISION_CHECK);

	Entity* target = AIAgentManager::GetInstance()->GetTargetEntity();
	const Vector2& curPosition = m_agent.GetPos();
	PhysicsManager::Raycast(CC_CALLBACK_3(AIAvoidTargetAction::OnRayCastCallback, this),
		curPosition,
		curPosition + m_agent.GetMoveDirection() * m_startAvoidingDistance);
}

bool AIAvoidTargetAction::OnRayCastCallback(cc::PhysicsWorld& world, const cc::PhysicsRayCastInfo& info, void* data)
{
	auto vec = info.normal.getPerp();
	Vector2 awayFromTargetAndCollision(vec.x, vec.y);
	awayFromTargetAndCollision.Normalize();
	m_agent.SetMoveDirection(awayFromTargetAndCollision);

	return true;
}

void AIAvoidTargetAction::OnAvoidCooldownExpired()
{	
	m_isAvoidCooldownActive = false;
	m_isAvoidTime = true;

	Utils::StartTimerWithCallback(&m_agent,
		CC_CALLBACK_0(AIAvoidTargetAction::OnAvoidTimerFinished, this),
		m_activeTime, GameConsts::ACTION_AI_AVOID);
}

void AIAvoidTargetAction::OnAvoidTimerFinished()
{
	m_isAvoidTime = false;
	m_isAvoidCooldownActive = true;

	Utils::StartTimerWithCallback(&m_agent,
		CC_CALLBACK_0(AIAvoidTargetAction::OnAvoidCooldownExpired, this),
		m_cooldownTime, GameConsts::ACTION_AI_AVOID);
}

void AIAvoidTargetAction::StopAvoiding()
{
	m_isAvoiding = false;
	m_agent.SetMoveDirection(Vector2::GetZero());
	AnimComponent* animComponent = m_agent.GetAnimComponent();
	if (!m_agent.IsProcessing() && !animComponent->IsCurrrentlyPlayingAnim(GameConsts::ANIM_TYPE_SIGNAL))
	{
		animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_IDLE);
	}
}

void AIAvoidTargetAction::StartAvoiding(const Entity* targetEntity)
{
	m_isAvoiding = true;
	Utils::StartTimerWithCallback(&m_agent,
		CC_CALLBACK_0(AIAvoidTargetAction::OnCollisionCheck, this),
		m_collisionCheckInterval, GameConsts::ACTION_COLLISION_CHECK);

	const Vector2& curPosition = m_agent.GetPos();
	PhysicsManager::Raycast(CC_CALLBACK_3(AIAvoidTargetAction::OnRayCastCallback, this),
		curPosition,
		curPosition + m_agent.GetMoveDirection() * m_startAvoidingDistance);

	Vector2 awayFromTarget = curPosition - targetEntity->GetPos();
	awayFromTarget.Normalize();
	m_agent.SetMoveDirection(awayFromTarget);

	AnimComponent* animComponent = m_agent.GetAnimComponent();
	if (!m_agent.IsProcessing() && !animComponent->IsCurrrentlyPlayingAnim(GameConsts::ANIM_TYPE_SIGNAL))
	{
		m_agent.GetAnimComponent()->PlayLoopingAnimation(GameConsts::ANIM_TYPE_RUN);
	}
}

AIAvoidTargetAction* AIAvoidTargetAction::Create(AIAgent& agent)
{
	AIAvoidTargetAction* avoidAction = new (std::nothrow) AIAvoidTargetAction(agent);
	if (avoidAction != nullptr)
	{
		avoidAction->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(avoidAction);
	}

	return avoidAction;
}

bool AIAvoidTargetAction::IsAvoiding() const
{
	return m_isAvoiding;
}

void AIAvoidTargetAction::LoadXMLData(const XMLElement* xmlElement)
{
	m_startAvoidingDistance = xmlElement->FloatAttribute(XMLConsts::START_AVOID_DISTANCE_ATTR);
	m_stopAvoidingDistance = xmlElement->FloatAttribute(XMLConsts::STOP_AVOID_DISTANCE_ATTR);
	m_collisionCheckInterval = xmlElement->FloatAttribute(XMLConsts::COLLISION_CHECK_TIME_INTERVAL_ATTR);

	String skipAvoidStates;
	XMLLoader::ReadXMLAttribute(xmlElement, XMLConsts::AI_SKIP_AVOID_STATES, skipAvoidStates);
	m_skipAvoidState = AState::GetStateFromString(skipAvoidStates);
}

void AIAvoidTargetAction::step(float deltaTime)
{
	Entity* targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	if (targetEntity != nullptr &&
		m_agent.GetCurrentAIState() != m_skipAvoidState &&
		m_agent.isVisible() &&
		!m_agent.IsProcessing() &&
		!m_isAvoidCooldownActive &&
		m_isAvoidTime)
	{
		if (IsTargetTooClose(targetEntity) && !m_isAvoiding)
		{
			StartAvoiding(targetEntity);
		}
		else if (IsTargetFarEnough(targetEntity) && m_isAvoiding)
		{
			StopAvoiding();
		}
	}
	else if(!m_agent.IsProcessing())
	{
		StopAvoiding();
	}
}

bool AIAvoidTargetAction::isDone() const
{
	return false;
}

