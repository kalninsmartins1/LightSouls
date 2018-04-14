#include "AIAnimComponent.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/AnimationUtils.h"

AIAnimComponent::AIAnimComponent(cocos2d::Sprite& sprite) :
	m_agentSprite(sprite)	
{
}

AIAnimComponent* AIAnimComponent::Create(cocos2d::Sprite& sprite)
{
	AIAnimComponent * ret = new (std::nothrow) AIAnimComponent(sprite);

	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

void AIAnimComponent::LoadConfig(XMLNode* pNode)
{
	for (XMLElement* pElem = pNode->FirstChildElement();
		pElem != nullptr; pElem = pElem->NextSiblingElement())
	{
		const std::string& animType = pElem->Attribute(XML_TYPE_ATTR);

		// Load specific animation based on its type
		if (animType == XML_ANIM_TYPE_RUN)
		{
			AnimationUtils::loadAnimationFrames(pElem, m_runFrames,
				m_timeBetweenRunFrames);
		}
		else if (animType == XML_ANIM_TYPE_IDLE)
		{
			AnimationUtils::loadAnimationFrames(pElem, m_idleFrames,
				m_timeBetweenIdleFrames);
		}
		else if (animType == XML_ANIM_TYPE_ATTACK)
		{
			AnimationUtils::loadAnimationFrames(pElem, m_attackFrames,
				m_timeBetweenAttackFrames);
		}
	}

	// Init parent sprite
	m_agentSprite.initWithSpriteFrame(m_idleFrames.at(0));
}

void AIAnimComponent::PlayIdleAnimation()
{
	AnimationUtils::startSpriteFrameAnimation(&m_agentSprite, m_idleFrames,
		m_timeBetweenIdleFrames);
}

void AIAnimComponent::PlayRunAnimation()
{
	AnimationUtils::startSpriteFrameAnimation(&m_agentSprite, m_runFrames,
		m_timeBetweenRunFrames);
}

void AIAnimComponent::playAttackAnimationWithCallback(std::function<void()> onFinsihed)
{
	AnimationUtils::startSpriteFrameAnimationWithCallback(&m_agentSprite, m_attackFrames,
		m_timeBetweenAttackFrames, onFinsihed);
}