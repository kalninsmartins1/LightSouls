#include "AgentAnimationComponent.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/AnimationUtils.h"

AgentAnimationComponent::AgentAnimationComponent(cocos2d::Sprite& sprite) :
	m_agentSprite(sprite)	
{
}

AgentAnimationComponent* AgentAnimationComponent::create(cocos2d::Sprite& sprite)
{
	AgentAnimationComponent * ret = new (std::nothrow) AgentAnimationComponent(sprite);

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

void AgentAnimationComponent::loadConfig(XMLNode* pNode)
{
	for (XMLElement* pElem = pNode->FirstChildElement();
		pElem != nullptr; pElem = pElem->NextSiblingElement())
	{
		const std::string& animType = pElem->Attribute(XML_TYPE_ATTR);

		// Load specific animation based on its type
		if (animType == XML_ANIM_TYPE_RUN)
		{
//			AnimationUtils::loadAnimationFrames(pElem, m_runFrames,
//				m_timeBetweenRunFrames);
		}
		else if (animType == XML_ANIM_TYPE_IDLE)
		{
			AnimationUtils::loadAnimationFrames(pElem, m_idleFrames,
				m_timeBetweenIdleFrames);
		}
		else if (animType == XML_ANIM_TYPE_HURT)
		{
//			AnimationUtils::loadAnimationFrames(pElem, m_hurtFrames,
//				m_timeBetweenIdleFrames);
		}
	}

	// Init parent sprite
	m_agentSprite.initWithSpriteFrame(m_idleFrames.at(0));
}

void AgentAnimationComponent::playIdleAnimation()
{
	AnimationUtils::startSpriteFrameAnimation(&m_agentSprite, m_idleFrames,
		m_timeBetweenIdleFrames);
}


