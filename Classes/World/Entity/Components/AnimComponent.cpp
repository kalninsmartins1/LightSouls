#include "AnimComponent.h"
#include "Utils/Utils.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/AnimationUtils.h"
#include "GameConsts.h"

NS_LIGHTSOULS_BEGIN

AnimComponent::AnimComponent(cocos2d::Sprite& ownerSprite) 
	: m_currentAnimId(-1)
	, m_ownerSprite(ownerSprite)	
{
}

AnimComponent* AnimComponent::Create(cocos2d::Sprite& sprite)
{
	AnimComponent * ret = new (std::nothrow) AnimComponent(sprite);

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

void AnimComponent::LoadConfig(tinyxml2::XMLNode* node)
{
	for (tinyxml2::XMLElement* element = node->FirstChildElement();
		element != nullptr; element = element->NextSiblingElement())
	{
		const String& animType = element->Attribute(XML_TYPE_ATTR);

		// Load specific animation based on its type
		AnimationData animationData;
		AnimationUtils::LoadAnimationFrames(element, animationData);
		m_animations[AnimationUtils::GetAnimId(animType)] = animationData;
	}

	// Init parent sprite
	m_ownerSprite.initWithSpriteFrame(m_animations[AnimationUtils::GetAnimId(ANIM_TYPE_IDLE)].frames.at(0));
}

void AnimComponent::PlayOneShotAnimation(int animationId, const AnimationCallback& callback)
{
	if (Utils::ContainsKey(m_animations, animationId))
	{
		AnimationUtils::StartSpriteFrameAnimationWithCallback(&m_ownerSprite,
			m_animations[animationId],
			callback);
		SetCurrentAnimId(animationId);
	}
	else
	{
		CCLOGERROR("Animation id %d not found !", animationId);
	}	
}

void AnimComponent::PlayLoopingAnimation(int animationId)
{	
	if (Utils::ContainsKey(m_animations, animationId))
	{
		AnimationUtils::StartSpriteFrameAnimation(&m_ownerSprite,
			m_animations[animationId]);
		SetCurrentAnimId(animationId);
	}
	else
	{
		CCLOGERROR("Animation id %d not found !", animationId);
	}
}

bool AnimComponent::IsCurrrentlyPlayingAnimation(const String& animName) const
{
	int animId = AnimationUtils::GetAnimId(animName);
	return animId == m_currentAnimId;
}

void AnimComponent::PlayLoopingAnimation(const String& animName)
{
	int animationId = AnimationUtils::GetAnimId(animName);
	PlayLoopingAnimation(animationId);
}

void AnimComponent::PlayOneShotAnimation(const String& animName, const AnimationCallback& callback)
{
	int animationId = AnimationUtils::GetAnimId(animName);
	PlayOneShotAnimation(animationId, callback);	
}

void AnimComponent::SetCurrentAnimId(int currentAnimId)
{
	m_currentAnimId = currentAnimId;
}

NS_LIGHTSOULS_END


