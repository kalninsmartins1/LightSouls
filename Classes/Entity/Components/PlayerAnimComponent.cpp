#include "PlayerAnimComponent.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
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
		pElem != nullptr; pElem = pElem->NextSiblingElement())
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

void PlayerAnimComponent::startAnimation(PlayerAnimationType type)
{
	switch (type)
	{
		case Idle:
			AnimationUtils::startSpriteFrameAnimation(m_pParent, m_idleFrames,
				m_timeBetweenIdleFrames);
			break;

		case Run: 
			AnimationUtils::startSpriteFrameAnimation(m_pParent, m_runFrames,
				m_timeBetweenRunFrames);
			break;

		case Dodge:
			AnimationUtils::startSpriteFrameAnimation(m_pParent, m_dodgeFrames,
				m_timeBetweenDodgeFrames);
			break;

		case Hurt: 
			AnimationUtils::startSpriteFrameAnimation(m_pParent, m_hurtFrames,
				m_timeBetweenHurtFrames);
			break;

		case Attack: 
			AnimationUtils::startSpriteFrameAnimation(m_pParent, m_attackFrames,
				m_timeBetweenAttackFrames);
			break;	
	}
}

float PlayerAnimComponent::getAnimationLengthInSeconds(PlayerAnimationType type) const
{
	float length = 0;
	switch (type)
	{
		case Idle: 
			length = m_idleFrames.size() * m_timeBetweenIdleFrames;
			break;

		case Run: 
			length = m_runFrames.size() * m_timeBetweenRunFrames;
			break;

		case Dodge:
			length = m_dodgeFrames.size() * m_timeBetweenDodgeFrames;
			break;

		case Hurt: 
			length = m_hurtFrames.size() * m_timeBetweenHurtFrames;
			break;

		case Attack:
			length = m_attackFrames.size() * m_timeBetweenAttackFrames;
			break;	
	}

	return length;
}
