#pragma once

#include "Classes/External/CocosEngine.h"
#include "World/Entity/Animation/BlurAnimation.h"

namespace tinyxml2
{
	class XMLNode;
	class XMLElement;
};

class String;
class Entity;
enum class AnimationKind;
struct AnimationData;

using AnimationCallback = std::function<void()>;
using XMLElement = tinyxml2::XMLElement;

enum class AnimStyle
{
	NONE,
	SIDE,
	UP,
	DOWN,
};

/*
 * Controls animation for sprite object. Depends on parent being of
 * type entity, because entity is everything that moves in the world.
 * And since animation is needed for movement, entity requirement is perfect fit.
 */
class AnimComponent: public cc::Component
{
public:
	bool					IsCurrrentlyPlayingAnim(const String& animName) const;
	bool					IsCurrrentlyPlayingAnim(int animId) const;
	bool					IsCurrentlyPlayingDirAnim(const String& animNam) const;
	bool					IsCurrentlyPlayingDirAnim(int animId) const;
	bool					IsCurrentlyPlayingDirAnim() const;
	bool					HasAnim(const String& animName) const;
	bool					HasAnim(int animId) const;

	const cocos2d::Sprite&		GetOwner() const;
	cocos2d::Node*				GetSpriteContainer() const;
	cocos2d::SpriteFrame*		GetCurrentSpriteFrame() const;

	static AnimComponent*	Create(cocos2d::Node& entity);

	// Load animation configuration from xml file
	void LoadConfig(const XMLElement& element);
	void PlayLoopingAnimation(const String& animName, bool shouldReverse = false);
	void PlayLoopingAnimation(int animationId, bool shouldReverse = false);
	void PlayOneShotAnimation(const String& animName, const AnimationCallback& callback);
	void PlayOneShotAnimation(int animationId, const AnimationCallback& callback);
	
	void PlayAttackAnimation(const AnimationCallback& callback);	
	void PlayLoopingDirectionalAnim(const String& animName, bool shouldReverse = false);
	void PlayOneShotDirectionalAnim(const String& animName, const AnimationCallback& callback);

	void			GoToNextAttackAnimation();
	void			ResetAttackAnimation();
	void			UpdateAnimState(const Entity& entity);
	virtual void	update(float deltaTime) override;

private:
	AnimComponent(cocos2d::Sprite& sprite);

private:
	int GetDirectionalAnimId(const String& animName) const;

	void SetCurrentAnimId(int currentAnimId);	
	void TransitionAttackAnimDirection(int firstAttackAnimId, int lastAttackAnimId);

	// Transition run anim in all directions when moving without stopping 
	void TransitionRunAnimDirection();
	void AddBlur(const AnimationData& data, int animationId);

private:
	int								m_curAnimId;
	int								m_firstAttackAnimId;
	int								m_lastAttackAnimId;
	int								m_curAttackAnimId;
	AnimStyle						m_currentAnimStyle;
	cc::Sprite&						m_sprite;
	std::map<int, AnimationData>	m_animations;
	BlurAnimation					m_blurAnimation;
};


