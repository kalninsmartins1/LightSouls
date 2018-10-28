#include "StateIsPlayerClose.h"
#include "World/Entity/AI/AIAgent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Entity/Components/Attack/GenericAttackComponent.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"
#include "World/Entity/AI/AIAgentManager.h"

NS_LIGHTSOULS_BEGIN

StateIsPlayerClose::StateIsPlayerClose(AIAgent& aiAgent)
	: AState(aiAgent)
	, m_animComponent(nullptr)
	, m_curProgress(EStateProgress::IN_PROGRESS)
	, m_closeDistance(50.0f)
{

}

void StateIsPlayerClose::OnEnter(AnimComponent* animComponent)
{
	AIAgent& agent = GetAgent();	

}

EStateProgress StateIsPlayerClose::OnStep()
{
	return m_curProgress;
}

void StateIsPlayerClose::OnExit()
{
	if (!GetAgent().IsProcessing() && m_animComponent != nullptr)
	{
		m_animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
	}
}

void StateIsPlayerClose::LoadXMLData(const XMLElement* xmlElement)
{
	AState::LoadXMLData(xmlElement);
	m_closeDistance = xmlElement->FloatAttribute(XML_AI_DISTANCE);
}

EAIState StateIsPlayerClose::GetStateType() const
{
	return EAIState::IS_PLAYER_CLOSE;
}

NS_LIGHTSOULS_END