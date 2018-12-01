#pragma once

#include "LightSoulsTypes.h"
#include "World/Entity/Animation/BlurAnimation.h"

namespace tinyxml2
{
	class XMLNode;
};

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
	bool					IsCurrrentlyPlayingAnim(const String& animName) const;
	bool					IsCurrrentlyPlayingAnim(int animId) const;
	bool					HasAnim(const String& animName) const;
	bool					HasAnim(int animId) const;
	const Entity&			GetOwnerEntity() const;
	cc::Node*				GetSpriteContainer() const;
	cc::SpriteFrame*		GetCurrentSpriteFrame() const;

	static AnimComponent*	Create(Entity& entity);

	// Load animation configuration from xml file
	void LoadConfig(tinyxml2::XMLNode* node);
	void PlayLoopingAnimation(const String& animName, bool shouldReverse = false, bool shouldBlur = false);
	void PlayLoopingAnimation(int animationId, bool shouldReverse = false, bool shouldBlur = false);
	void PlayOneShotAnimation(const String& animName, const AnimationCallback& callback, bool shouldBlur = false);
	void PlayOneShotAnimation(int animationId, const AnimationCallback& callback, bool shouldBlur = false);
	void PlayAttackAnimation(const AnimationCallback& callback);

	void			GoToNextAttackAnimation();
	void			ResetAttackAnimation();
	virtual void	update(float deltaTime) override;

private:
	AnimComponent(Entity& sprite);

private:	
	void SetCurrentAnimId(int currentAnimId);

	void UpdateAttackAnimState();
	void TransitionAttackAnimDirection(AttackAnimStyle style, int firstAttackAnimId, int lastAttackAnimId);
	void AddBlur(const AnimationData& data, int animationId);

private:
	int								m_curAnimId;
	int								m_firstAttackAnimId;
	int								m_lastAttackAnimId;
	int								m_curAttackAnimId;
	AttackAnimStyle					m_currentAttackStyle;
	Entity&							m_entity;
	std::map<int, AnimationData>	m_animations;
	BlurAnimation					m_blurAnimation;
};


