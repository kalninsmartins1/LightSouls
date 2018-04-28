#pragma once

#include "2d/CCSprite.h"
#include "World/Entity/Entity.h"
#include "World/Entity/Components/LongSwordAttackComponent.h"

NS_LIGHTSOULS_BEGIN

class AnimComponent;
class GameInput;

class Player: public Entity
{

public:
	Player();

	static const String& GetOnHealthChangedEvent();

	void SetTimeBetweenComboInput(float timeBetweenComboInput);
	
	static Player*	Create(const String& pPathToXML);
	bool			Init(const String& pathToXML);
	
	virtual void	update(float deltaTime) override;

protected:
	virtual void DispatchOnHealthChangedEvent() override;

private:
	void OnDodgeFinished();
	void OnAttackFinished();
	void OnLightAttackComboExpired();
	
	void ManageInput();
	void LightAttack();
	void PerformDodge();
	void PlayRunOrIdleAnimation() const;
	void OnContactBegin(const cocos2d::PhysicsBody* otherBody);

	// Returns negative value if time for combo has expired
	float GetSecondsForValidLighAttackCombo() const;

private:
	static const String	s_eventOnPlayerHealthChanged;
	AnimComponent*		m_animComponent;
	AttackComponent*			m_attackComponent;
	Vector2						m_lastValidMoveDirection;

	bool				m_isAttackComboDelayExpired;
	float				m_timeBetweenComboInput;
	long				m_lastTimePerformedLightAttack;
	int					m_curAttackAnimId;
	const int			m_lastAttackAnimId;
	const int			m_firstAttackAnimId;
};

NS_LIGHTSOULS_END
