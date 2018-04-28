#pragma once

#include "LightSoulsTypes.h"

namespace tinyxml2 {
	class XMLNode;
}

NS_LIGHTSOULS_BEGIN

enum class AnimationKind;
struct AnimationData;

using AnimationCallback = std::function<void()>;

/*
 * Controls animation for sprite object. Depends on parent being of
 * type sprite. Since otherwise there is nothing to animate.
 */
class AnimComponent: public cocos2d::Component
{
public:
	bool IsCurrrentlyPlayingAnimation(const String& animName) const;

	static AnimComponent* Create(cocos2d::Sprite& sprite);

	// Load animation configuration from xml file
	void LoadConfig(tinyxml2::XMLNode* node);

	void PlayLoopingAnimation(const String& animName);
	void PlayLoopingAnimation(int animationId);
	void PlayOneShotAnimation(const String& animName, const AnimationCallback& callback = nullptr);
	void PlayOneShotAnimation(int animationId, const AnimationCallback& callback = nullptr);

private:
	AnimComponent(cocos2d::Sprite& sprite);

private:	
	void SetCurrentAnimId(int currentAnimId);

private:
	int							m_currentAnimId;
	cocos2d::Sprite&			m_ownerSprite;
	std::map<int, AnimationData>	m_animations;
};

NS_LIGHTSOULS_END
