#include "AIAvoidTargetAction.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/XML/XMLLoader.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Physics/PhysicsManager.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/StateMachine/States/AState.h"

NS_LIGHTSOULS_BEGIN

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
		m_activeTime, ACTION_AI_AVOID);
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
		m_collisionCheckInterval, ACTION_COLLISION_CHECK);

	Entity* target = AIAgentManager::GetInstance()->GetTargetEntity();
	const Vector2& curPosition = m_agent.getPosition();
	PhysicsManager::Raycast(CC_CALLBACK_3(AIAvoidTargetAction::OnRayCastCallback, this),
		curPosition,
		curPosition + m_agent.GetHeading() * m_startAvoidingDistance);
}

bool AIAvoidTargetAction::OnRayCastCallback(cocos2d::PhysicsWorld& world, const cocos2d::PhysicsRayCastInfo& info, void* data)
{
	Vector2 awayFromTargetAndCollision = info.normal.getPerp();
	awayFromTargetAndCollision.normalize();
	m_agent.SetMoveDirection(awayFromTargetAndCollision);

	return true;
}

void AIAvoidTargetAction::OnAvoidCooldownExpired()
{	
	m_isAvoidCooldownActive = false;
	m_isAvoidTime = true;

	Utils::StartTimerWithCallback(&m_agent,
		CC_CALLBACK_0(AIAvoidTargetAction::OnAvoidTimerFinished, this),
		m_activeTime, ACTION_AI_AVOID);	
}

void LightSouls::AIAvoidTargetAction::OnAvoidTimerFinished()
{
	m_isAvoidTime = false;
	m_isAvoidCooldownActive = true;

	Utils::StartTimerWithCallback(&m_agent,
		CC_CALLBACK_0(AIAvoidTargetAction::OnAvoidCooldownExpired, this),
		m_cooldownTime, ACTION_AI_AVOID);
}

void AIAvoidTargetAction::StopAvoiding()
{
	m_isAvoiding = false;
	m_agent.SetMoveDirection(Vector2::ZERO);
}

void AIAvoidTargetAction::StartAvoiding(const Entity* targetEntity)
{
	m_isAvoiding = true;
	Utils::StartTimerWithCallback(&m_agent,
		CC_CALLBACK_0(AIAvoidTargetAction::OnCollisionCheck, this),
		m_collisionCheckInterval, ACTION_COLLISION_CHECK);

	const Vector2& curPosition = m_agent.getPosition();
	PhysicsManager::Raycast(CC_CALLBACK_3(AIAvoidTargetAction::OnRayCastCallback, this),
		curPosition,
		curPosition + m_agent.GetHeading() * m_startAvoidingDistance);

	Vector2 awayFromTarget = m_agent.getPosition() - targetEntity->getPosition();
	awayFromTarget.normalize();
	m_agent.SetMoveDirection(awayFromTarget);
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

void AIAvoidTargetAction::LoadXMLData(const XMLElement* xmlElement)
{
	m_startAvoidingDistance = xmlElement->FloatAttribute(XML_START_AVOID_DISTANCE_ATTR);
	m_stopAvoidingDistance = xmlElement->FloatAttribute(XML_STOP_AVOID_DISTANCE_ATTR);
	m_collisionCheckInterval = xmlElement->FloatAttribute(XML_COLLISION_CHECK_TIME_INTERVAL_ATTR);

	String skipAvoidStates;
	XMLLoader::ReadXMLAttribute(xmlElement, XML_AI_SKIP_AVOID_STATES, skipAvoidStates);
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

NS_LIGHTSOULS_END