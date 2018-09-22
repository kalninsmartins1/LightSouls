#include "StateSignaling.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Entity/Components/Attack/GenericAttackComponent.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"
#include "World/Entity/AI/AIAgentManager.h"

NS_LIGHTSOULS_BEGIN

StateSignaling::StateSignaling(AIAgent& aiAgent)
	: AState(aiAgent)
	, m_animComponent(nullptr)
	, m_curProgress(EStateProgress::IN_PROGRESS)
{

}

void StateSignaling::OnEnter(AnimComponent* animComponent)
{
	AIAgent& agent = GetAgent();
	const Vector2& targetEntityPoisition = AIAgentManager::GetInstance()->GetTargetEntity()->getPosition();
	if (agent.GetAttackComponent()->IsReadyToAttack(targetEntityPoisition))
	{
		m_animComponent = animComponent;
		m_animComponent->PlayLoopingAnimation(ANIM_TYPE_SIGNAL);
		m_curProgress = EStateProgress::IN_PROGRESS;

		Utils::StartTimerWithCallback(&agent,
			CC_CALLBACK_0(StateSignaling::OnFinishedSignaling, this), m_signalTime);
	}
	else
	{
		m_curProgress = EStateProgress::FAILED;
	}	
}

EStateProgress StateSignaling::OnStep()
{
	return m_curProgress;
}

void StateSignaling::OnExit()
{
	if (m_animComponent != nullptr)
	{
		m_animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
	}
}

void StateSignaling::LoadXMLData(const XMLElement* xmlElement)
{
	AState::LoadXMLData(xmlElement);
	m_signalTime = xmlElement->FloatAttribute(XML_TIME_ATTR);
}

void StateSignaling::OnFinishedSignaling()
{
	m_curProgress = EStateProgress::DONE;
}

EAIState StateSignaling::GetStateType() const
{
	return EAIState::SIGNALING;
}

NS_LIGHTSOULS_END