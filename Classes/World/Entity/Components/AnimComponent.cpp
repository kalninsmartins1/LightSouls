#include "AnimComponent.h"
#include "Utils/Utils.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/AnimationUtils.h"
#include "GameConsts.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

AnimComponent::AnimComponent(Entity& ownerSprite) 
	: m_curAnimId(-1)
	, m_entity(ownerSprite)
	, m_firstAttackAnimId(AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_ONE_FORWARD))
	, m_lastAttackAnimId(AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_FIVE_FORWARD))
	, m_curAttackAnimId(m_firstAttackAnimId)
	, m_currentAttackStyle(AttackAnimStyle::FORWARD)
	, m_animations()
{

}

AnimComponent* AnimComponent::Create(Entity& sprite)
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
	for (tinyxml2::XMLElement* spriteSheetNode = node->FirstChildElement();
		spriteSheetNode != nullptr; spriteSheetNode = spriteSheetNode->NextSiblingElement())
	{
		auto spriteCache = cocos2d::SpriteFrameCache::getInstance();
		const char* plistPath = spriteSheetNode->Attribute(XML_ANIM_PLIST_PATH_ATTR);
		spriteCache->addSpriteFramesWithFile(plistPath);

		for (tinyxml2::XMLElement* animNode = spriteSheetNode->FirstChildElement();
			animNode != nullptr; animNode = animNode->NextSiblingElement())
		{
			const String& animType = animNode->Attribute(XML_TYPE_ATTR);

			// Load specific animation based on its type
			AnimationData animationData;
			AnimationUtils::LoadAnimationFrames(animNode, animationData);
			m_animations[AnimationUtils::GetAnimId(animType)] = animationData;
		}
	}

	// Init parent sprite
	m_entity.initWithSpriteFrame(m_animations[AnimationUtils::GetAnimId(ANIM_TYPE_IDLE)].frames.at(0));
}

void AnimComponent::PlayOneShotAnimation(int animationId, const AnimationCallback& callback)
{
#if LIGHTSOULS_ANIM_DEBUG
	CCLOG("PlayOneShotAnimation: %d %s", animationId, getOwner()->getName().c_str());
#endif

	if (Utils::ContainsKey(m_animations, animationId))
	{
		AnimationUtils::StartSpriteFrameAnimationWithCallback(&m_entity,
			m_animations[animationId],
			callback);
		SetCurrentAnimId(animationId);
	}
	else
	{
		CCLOGERROR("Animation id %d not found !", animationId);
	}	
}

void AnimComponent::PlayAttackAnimation(const AnimationCallback& callback)
{
	PlayOneShotAnimation(m_curAttackAnimId, callback);
}

void AnimComponent::GoToNextAttackAnimation()
{
	// Wrap the index within valid values
	Utils::WrapValue(++m_curAttackAnimId, m_firstAttackAnimId, m_lastAttackAnimId);
}

void AnimComponent::ResetAttackAnimation()
{
	m_curAttackAnimId = m_firstAttackAnimId;
}

void AnimComponent::update(float deltaTime)
{
	UpdateAttackAnimState();
}

void AnimComponent::PlayLoopingAnimation(int animationId)
{
#if LIGHTSOULS_ANIM_DEBUG
	CCLOG("PlayLoopingAnimation: %d %s", animationId, getOwner()->getName().c_str());
#endif
	if (Utils::ContainsKey(m_animations, animationId))
	{
		AnimationUtils::StartSpriteFrameAnimation(&m_entity,
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
	return animId == m_curAnimId;
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
	m_curAnimId = currentAnimId;
}

void AnimComponent::UpdateAttackAnimState()
{
	const Vector2& heading = m_entity.GetHeading();
	if (abs(heading.x) > 0.0f)
	{
		m_currentAttackStyle = AttackAnimStyle::FORWARD;
		m_firstAttackAnimId = AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_ONE_FORWARD);
		m_lastAttackAnimId = AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_FIVE_FORWARD);
		m_curAttackAnimId = m_firstAttackAnimId;
	}
	else if (heading.y > 0.0f)
	{
		m_currentAttackStyle = AttackAnimStyle::UPWARD;
		m_firstAttackAnimId = AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_ONE_UPWARD);
		m_lastAttackAnimId = AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_FIVE_UPWARD);
		m_curAttackAnimId = m_firstAttackAnimId;
	}
	else if (heading.y < 0.0f)
	{
		m_currentAttackStyle = AttackAnimStyle::DOWNWARD;
		m_firstAttackAnimId = AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_ONE_DOWNWARD);
		m_lastAttackAnimId = AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_FIVE_DOWNWARD);
		m_curAttackAnimId = m_firstAttackAnimId;
	}
}

NS_LIGHTSOULS_END

