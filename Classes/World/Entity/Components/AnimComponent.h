#pragma once

#include "LightSoulsTypes.h"

namespace tinyxml2 {
	class XMLNode;
}

NS_LIGHTSOULS_BEGIN

class Entity;
enum class AnimationKind;
struct AnimationData;

using AnimationCallback = std::function<void()>;

enum class AttackAnimStyle
{
	NONE,
	FORWARD,
	UPWARD,
	DOWNWARD,
};

/*
 * Controls animation for sprite object. Depends on parent being of
 * type entity, because entity is everything that moves in the world.
 * And since animation is needed for movement, entity requirement is perfect fit.
 */
class AnimComponent: public cocos2d::Component
{
public:
	bool IsCurrrentlyPlayingAnimation(const String& animName) const;

	static AnimComponent* Create(Entity& entity);

	// Load animation configuration from xml file
	void LoadConfig(tinyxml2::XMLNode* node);

	void PlayLoopingAnimation(const String& animName);
	void PlayLoopingAnimation(int animationId);
	void PlayOneShotAnimation(const String& animName, const AnimationCallback& callback);
	void PlayOneShotAnimation(int animationId, const AnimationCallback& callback);
	void PlayAttackAnimation(const AnimationCallback& callback);

	void GoToNextAttackAnimation();
	void ResetAttackAnimation();

	virtual void update(float deltaTime) override;

private:
	AnimComponent(Entity& sprite);

private:	
	void SetCurrentAnimId(int currentAnimId);

	void UpdateAttackAnimState();
	void TransitionAttackAnimDirection(AttackAnimStyle style, int firstAttackAnimId, int lastAttackAnimId);

private:
	int								m_curAnimId;
	int								m_firstAttackAnimId;
	int								m_lastAttackAnimId;
	int								m_curAttackAnimId;
	AttackAnimStyle					m_currentAttackStyle;
	Entity&							m_entity;
	std::map<int, AnimationData>	m_animations;
};

NS_LIGHTSOULS_END
