#include "PlayerAnimComponent.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
#include "3rdPartyLibs/tinyxml2.h"
#include "Utils/AnimationUtils.h"

using namespace cocos2d;

void PlayerAnimComponent::setOwner(cocos2d::Node *owner)
{
	Component::setOwner(owner);
	m_pParent = (Sprite*)owner;

	if (m_pParent == nullptr)
	{
		cocos2d::log("Error: PlayerAimation component is not attached to Sprite !");
	}
}

void PlayerAnimComponent::loadConfig(tinyxml2::XMLNode* pNode)
{
	for (tinyxml2::XMLElement* pElem = pNode->FirstChildElement();
		pElem->NextSiblingElement() != nullptr; pElem = pElem->NextSiblingElement())
	{
		const char* animType = pElem->Attribute("type");

		// Load specific animation based on its type
		if (Utils::isStrEqual(animType, "Running"))
		{
			AnimationUtils::loadAnimationFrames(pElem, m_runFrames, 
				m_timeBetweenRunFrames);
		}
		else if (Utils::isStrEqual(animType, "Idle"))
		{
			AnimationUtils::loadAnimationFrames(pElem, m_idleFrames, 
				m_timeBetweenIdleFrames);
		}
		else if (Utils::isStrEqual(animType, "Hurt"))
		{
			AnimationUtils::loadAnimationFrames(pElem, m_hurtFrames, 
				m_timeBetweenIdleFrames);
		}
		else if (Utils::isStrEqual(animType, "Dodge"))
		{
			AnimationUtils::loadAnimationFrames(pElem, m_dodgeFrames, 
				m_timeBetweenDodgeFrames);
		}
		else if (Utils::isStrEqual(animType, "Attack"))
		{
			AnimationUtils::loadAnimationFrames(pElem, m_attackFrames, 
				m_timeBetweenAttackFrames);
		}
	}

	// Init parent sprite
	m_pParent->initWithSpriteFrame(m_idleFrames.at(0));
}

void PlayerAnimComponent::startRunAnimation()
{
	AnimationUtils::startSpriteFrameAnimation(m_pParent, m_runFrames,
		m_timeBetweenRunFrames);
}

void PlayerAnimComponent::startDodgeAnimation()
{	
	AnimationUtils::startSpriteFrameAnimation(m_pParent, m_dodgeFrames, 
		m_timeBetweenDodgeFrames);
}

void PlayerAnimComponent::startIdleAnimation()
{		
	AnimationUtils::startSpriteFrameAnimation(m_pParent, m_idleFrames, 
		m_timeBetweenIdleFrames);
}

void PlayerAnimComponent::startAttackAnimation()
{	
	AnimationUtils::startSpriteFrameAnimation(m_pParent, m_attackFrames, 
		m_timeBetweenAttackFrames);
}
