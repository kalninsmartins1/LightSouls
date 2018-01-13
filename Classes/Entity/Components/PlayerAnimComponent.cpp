#include "PlayerAnimComponent.h"
#include "Utils/Utils.h"
#include "Utils/AnimationUtils.h"
#include "tinyxml2/tinyxml2.h"
#include "Entity/Player/Player.h"

using namespace cocos2d;


PlayerAnimComponent::PlayerAnimComponent(Sprite& player) :
	m_playerSprite(player)
{
}

PlayerAnimComponent* PlayerAnimComponent::create(Sprite& player)
{
	PlayerAnimComponent * ret = new (std::nothrow) PlayerAnimComponent(player);

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
		else if (Utils::isStrEqual(animType, "LightAttackOne"))
		{
			AnimationUtils::loadAnimationFrames(pElem, m_lightAttackOneFrames,
				m_timeBetweenLightAttackOneFrames);
		}
		else if (Utils::isStrEqual(animType, "LightAttackTwo"))
		{
			AnimationUtils::loadAnimationFrames(pElem, m_lightAttackTwoFrames,
				m_timeBetweenLightAttackTwoFrames);
		}
		else if (Utils::isStrEqual(animType, "LightAttackThree"))
		{
			AnimationUtils::loadAnimationFrames(pElem, m_lightAttackThreeFrames,
				m_timeBetweenLightAttackThreeFrames);
		}
		else if (Utils::isStrEqual(animType, "LightAttackFour"))
		{
			AnimationUtils::loadAnimationFrames(pElem, m_lightAttackFourFrames,
				m_timeBetweenLightAttackFourFrames);
		}
		else if (Utils::isStrEqual(animType, "LightAttackFive"))
		{
			AnimationUtils::loadAnimationFrames(pElem, m_lightAttackFiveFrames,
				m_timeBetweenLightAttackFiveFrames);
		}
	}

	// Init parent sprite
	m_playerSprite.initWithSpriteFrame(m_idleFrames.at(0));
}

void PlayerAnimComponent::playLightAttackAnimation(LightAttackStage stage,
	const AnimationCallback& callback)
{
	switch (stage)
	{
	case LightAttackStage::ONE:
		AnimationUtils::startSpriteFrameAnimationWithCallback(&m_playerSprite,
			m_lightAttackOneFrames,
			m_timeBetweenLightAttackOneFrames, 
			callback);
		break;

	case LightAttackStage::TWO:
		AnimationUtils::startSpriteFrameAnimationWithCallback(&m_playerSprite,
			m_lightAttackTwoFrames,
			m_timeBetweenLightAttackTwoFrames,
			callback);
		break;

	case LightAttackStage::THREE:
		AnimationUtils::startSpriteFrameAnimationWithCallback(&m_playerSprite,
			m_lightAttackThreeFrames,
			m_timeBetweenLightAttackThreeFrames,
			callback);
		break;

	case LightAttackStage::FOUR:
		AnimationUtils::startSpriteFrameAnimationWithCallback(&m_playerSprite,
			m_lightAttackFourFrames,
			m_timeBetweenLightAttackFourFrames,
			callback);
		break;

	case LightAttackStage::FIVE:
		AnimationUtils::startSpriteFrameAnimationWithCallback(&m_playerSprite,
			m_lightAttackFiveFrames,
			m_timeBetweenLightAttackFiveFrames,
			callback);
		break;

	default:
		CCLOGERROR("PlayerAnimComponent: [playLightAttackAnimation] invalid light animation stage !");
		break;
	}
}

void PlayerAnimComponent::playRunAnimation()
{
	AnimationUtils::startSpriteFrameAnimation(&m_playerSprite, m_runFrames,
		m_timeBetweenRunFrames);
}

void PlayerAnimComponent::playIdleAnimation()
{
	AnimationUtils::startSpriteFrameAnimation(&m_playerSprite, m_idleFrames,
		m_timeBetweenIdleFrames);
}

void PlayerAnimComponent::playDodgeAnimation()
{
	AnimationUtils::startSpriteFrameAnimation(&m_playerSprite, m_dodgeFrames,
		m_timeBetweenDodgeFrames);
}

void PlayerAnimComponent::playHurtAnimation()
{
	AnimationUtils::startSpriteFrameAnimation(&m_playerSprite, m_hurtFrames,
		m_timeBetweenHurtFrames);
}