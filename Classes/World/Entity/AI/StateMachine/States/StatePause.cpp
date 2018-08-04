#include "StatePause.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"

NS_LIGHTSOULS_BEGIN

StatePause::StatePause(AIAgent& aiAgent)
	: AState(aiAgent)	
	, m_targetEntity(nullptr)
	, m_curProgress(EStateProgress::IN_PROGRESS)
	, m_pauseTime(0.0f)
{

}

void StatePause::OnEnter(AnimComponent* animComponent)
{
	animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
	m_curProgress = EStateProgress::IN_PROGRESS;
	m_targetEntity = AIAgentManager::GetInstance()->GetTargetEntity();
	
	Utils::StartTimerWithCallback(&GetAgent(),
			CC_CALLBACK_0(StatePause::OnPauseExpired, this), m_pauseTime);

}

EStateProgress StatePause::OnStep()
{
	return m_curProgress;
}

void StatePause::OnExit()
{

}

void StatePause::LoadXMLData(const XMLElement* xmlElement)
{
	AState::LoadXMLData(xmlElement);
	m_pauseTime = xmlElement->FloatAttribute(XML_TIME_ATTR);	
}

void StatePause::OnPauseExpired()
{
	m_curProgress = EStateProgress::DONE;
}

EAIState StatePause::GetStateType() const
{
	return EAIState::PAUSE;
}

NS_LIGHTSOULS_END