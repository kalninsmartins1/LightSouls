#include "AnimComponent.h"
#include "Utils/Utils.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/AnimationUtils.h"
#include "GameConsts.h"
#include "World/Entity/Entity.h"
#include "Classes/Core/String/String.h"

using namespace cocos2d;

AnimComponent::AnimComponent(Sprite& ownerSprite) 
	: m_curAnimId(-1)
	, m_sprite(ownerSprite)
	, m_firstAttackAnimId(AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_ATTACK_COMBO_ONE_SIDE))
	, m_lastAttackAnimId(AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_ATTACK_COMBO_TWO_SIDE))
	, m_curAttackAnimId(m_firstAttackAnimId)
	, m_currentAnimStyle(AnimStyle::SIDE)
	, m_animations()
	, m_blurAnimation(*this)
{

}

int AnimComponent::GetDirectionalAnimId(const String& animName) const
{
	int animationId = AnimationUtils::GetAnimId(animName);
	if (animationId != -1)
	{
		switch (m_currentAnimStyle)
		{
			/* We are relaying that directional animations are defined in certain order and
				comes in pairs of 3, but since its a core level thing it should be fine.
			*/
			case AnimStyle::UP:
				animationId += 1;
				break;

			case AnimStyle::DOWN:
				animationId += 2;
				break;
		}
	}

	return animationId;
}

AnimComponent* AnimComponent::Create(cc::Node& node)
{
	cc::Sprite* sprite = static_cast<cc::Sprite*>(&node);
	AnimComponent* ret = nullptr;

	if (sprite != nullptr)
	{
		ret = new (std::nothrow) AnimComponent(*sprite);
	}

	if (ret != nullptr && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

void AnimComponent::LoadConfig(const XMLElement& element)
{
	for (const XMLElement* spriteSheetElement = element.FirstChildElement();
		spriteSheetElement != nullptr; spriteSheetElement = spriteSheetElement->NextSiblingElement())
	{
		auto spriteCache = cocos2d::SpriteFrameCache::getInstance();
		const char* plistPath = spriteSheetElement->Attribute(XMLConsts::ANIM_PLIST_PATH_ATTR);
		spriteCache->addSpriteFramesWithFile(plistPath);

		for (const XMLElement* animElement = spriteSheetElement->FirstChildElement();
			animElement != nullptr; animElement = animElement->NextSiblingElement())
		{
			const String& animType = animElement->Attribute(XMLConsts::TYPE_ATTR);

			// Load specific animation based on its type
			AnimationData animationData;
			AnimationUtils::LoadAnimationFrames(animElement, animationData);
			m_animations[AnimationUtils::GetAnimId(animType)] = animationData;
		}
	}

	// Init parent sprite
	int animId = -1;
	if (HasAnim(GameConsts::ANIM_TYPE_IDLE))
	{
		animId = AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_IDLE);
	}
	else if (HasAnim(GameConsts::ANIM_TYPE_IDLE_DIR))
	{
		animId = AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_IDLE_DIR);
	}

	if (animId != -1)
	{
		m_sprite.initWithSpriteFrame(m_animations[animId].frames.at(0));
	}
	else
	{
		CC_ASSERT(false && "AnimComponent::LoadConfig - Error IDLE animation not found !");
	}
}

void AnimComponent::PlayOneShotAnimation(int animationId, const AnimationCallback& callback)
{
#if LIGHTSOULS_ANIM_DEBUG
	String animName;
	AnimationUtils::GetAnimName(animationId, animName);
	CCLOG("PlayOneShotAnimation: %s %s", animName.c_str(), getOwner()->getName().c_str());
#endif	

	if (HasAnim(animationId))
	{
		const AnimationData& data = m_animations[animationId];
		AnimationUtils::StartSpriteFrameAnimationWithCallback(&m_sprite, data, callback);
		SetCurrentAnimId(animationId);

		if (data.shouldBlur)
		{
			AddBlur(data, animationId);
		}
	}
	else
	{
		// Still call the callback
		if (callback != nullptr)
		{
			callback();
		}
	}
}

void AnimComponent::PlayAttackAnimation(const AnimationCallback& callback)
{
	PlayOneShotAnimation(m_curAttackAnimId, callback);
}

void AnimComponent::PlayLoopingDirectionalAnim(const String& animName, bool shouldReverse /*= false*/)
{
	int animationId = GetDirectionalAnimId(animName);
	if (animationId != -1)
	{
		PlayLoopingAnimation(animationId, shouldReverse);		
	}
	else
	{
		Utils::AssertWithStrFormat(false, "AnimComponent::PlayLoopingDirectionalAnim - animation %s not found", animName);
	}
}

void AnimComponent::PlayOneShotDirectionalAnim(const String& animName, const AnimationCallback& callback)
{
	int animationId = GetDirectionalAnimId(animName);
	if (animationId != -1)
	{
		PlayOneShotAnimation(animationId, callback);
	}
	else
	{
		Utils::AssertWithStrFormat(false, "AnimComponent::PlayOneShotDirectionalAnim - animation %s not found", animName);
	}
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
	m_blurAnimation.Update(deltaTime);
}

void AnimComponent::PlayLoopingAnimation(int animationId, bool shouldReverse)
{
#if LIGHTSOULS_ANIM_DEBUG
	String animName;
	AnimationUtils::GetAnimName(animationId, animName);
	CCLOG("PlayLoopingAnimation: %s %s", animName.c_str(), getOwner()->getName().c_str());
#endif	

	if (HasAnim(animationId))
	{
		const AnimationData& data = m_animations[animationId];
		AnimationUtils::StartSpriteFrameAnimation(&m_sprite, data, shouldReverse);
		SetCurrentAnimId(animationId);
		
		if (data.shouldBlur)
		{
			AddBlur(data, animationId);
		}
	}
}

bool AnimComponent::IsCurrrentlyPlayingAnim(const String& animName) const
{
	int animId = AnimationUtils::GetAnimId(animName);
	return IsCurrrentlyPlayingAnim(animId);
}

bool AnimComponent::IsCurrrentlyPlayingAnim(int animId) const
{
	return animId == m_curAnimId;
}

bool AnimComponent::IsCurrentlyPlayingDirAnim(const String& animNam) const
{
	int animId = AnimationUtils::GetAnimId(animNam);
	return IsCurrentlyPlayingDirAnim(animId);
}

bool AnimComponent::IsCurrentlyPlayingDirAnim(int animId) const
{
	// Directional animations can be 3 different kinds side, up or down and they are stored in sequence
	int diff = m_curAnimId - animId;
	return diff >= 0 && diff <= 2;
}

bool AnimComponent::IsCurrentlyPlayingDirAnim() const
{
	return IsCurrentlyPlayingDirAnim(GameConsts::ANIM_TYPE_RUN_DIR) ||
		IsCurrentlyPlayingDirAnim(GameConsts::ANIM_TYPE_HURT_DIR) ||
		IsCurrentlyPlayingDirAnim(GameConsts::ANIM_TYPE_IDLE_DIR) ||
		IsCurrentlyPlayingDirAnim(GameConsts::ANIM_TYPE_DODGE_DIR);
}

bool AnimComponent::HasAnim(const String& animName) const
{
	int animationId = AnimationUtils::GetAnimId(animName);
	return HasAnim(animationId);
}

bool AnimComponent::HasAnim(int animId) const
{
	return Utils::ContainsKey(m_animations, animId);
}

const cc::Sprite& AnimComponent::GetOwner() const
{
	return m_sprite;
}

cocos2d::Node* AnimComponent::GetSpriteContainer() const
{
	return m_sprite.getParent();
}

cc::SpriteFrame* AnimComponent::GetCurrentSpriteFrame() const
{
	return m_sprite.getSpriteFrame();
}

void AnimComponent::PlayLoopingAnimation(const String& animName, bool shoudlRevers)
{
	int animationId = AnimationUtils::GetAnimId(animName);
	PlayLoopingAnimation(animationId, shoudlRevers);
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

void AnimComponent::UpdateAnimState(const Entity& entity)
{
	const Vector2& heading = entity.GetMoveDirection();
	const float absoluteXValue = abs(heading.GetX());
	const float absoluteYValue = abs(heading.GetY());

	if (abs(heading.GetX()) > 0.0f && absoluteXValue > absoluteYValue)
	{		
		m_currentAnimStyle = AnimStyle::SIDE;
		TransitionAttackAnimDirection(
			AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_ATTACK_COMBO_ONE_SIDE),
			AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_ATTACK_COMBO_TWO_SIDE));
	}
	else if (heading.GetY() > 0.0f)
	{		
		m_currentAnimStyle = AnimStyle::UP;
		TransitionAttackAnimDirection(
			AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_ATTACK_COMBO_ONE_UP),
			AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_ATTACK_COMBO_TWO_UP));
	}
	else if (heading.GetY() < 0.0f)
	{	
		m_currentAnimStyle = AnimStyle::DOWN;
		TransitionAttackAnimDirection(
			AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_ATTACK_COMBO_ONE_DOWN),
			AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_ATTACK_COMBO_TWO_DOWN));
	}

	TransitionRunAnimDirection();
}

void AnimComponent::TransitionAttackAnimDirection(int firstAttackAnimId, int lastAttackAnimId)
{		
	int curAttackIdDif = m_curAttackAnimId - m_firstAttackAnimId;

	m_firstAttackAnimId = firstAttackAnimId;
	m_curAttackAnimId = m_firstAttackAnimId + curAttackIdDif;
	m_lastAttackAnimId = lastAttackAnimId;
}

void AnimComponent::TransitionRunAnimDirection()
{	
	const String runAnimName = GameConsts::ANIM_TYPE_RUN_DIR;
	if (IsCurrentlyPlayingDirAnim(runAnimName))
	{
		int animId = GetDirectionalAnimId(runAnimName);
		if (m_curAnimId != animId)
		{
			PlayLoopingDirectionalAnim(runAnimName);
		}
	}
}

void AnimComponent::AddBlur(const AnimationData& data, int animationId)
{
	m_blurAnimation.AddFrame(data.snapShotDelay, animationId);
	m_blurAnimation.AddFrame(data.snapShotDelay, animationId, data.secondFrameDelay);
	m_blurAnimation.AddFrame(data.snapShotDelay, animationId, data.secondFrameDelay * 2.0f);
}
