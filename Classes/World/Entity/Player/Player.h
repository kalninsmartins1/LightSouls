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

public:
	static const String& GetEventOnHealthChanged();
	static const String& GetEventOnStaminaChanged();

	void SetTimeBetweenComboInput(float timeBetweenComboInput);
	void SetDodgeStaminaConsumption(float dodgeStaminaConumption);
	void SetDodgeSpeed(float dodgeSpeed);
	void SetDodgeTime(float dodgeTime);

	static Player*	Create(const String& pathToXML);
	bool			Init(const String& pathToXML);
	
	virtual void	update(float deltaTime) override;

protected:
	virtual void DispatchOnHealthChangedEvent() override;
	virtual void DispatchOnStaminaChangedEvent() override;

private:

	// Returns negative value if time for combo has expired
	float GetSecondsForValidLighAttackCombo() const;

	void StartDodging();
	void StopDodging();

	void OnDodgeFinished();
	void OnAttackFinished();
	void OnLightAttackComboExpired();
	
	void UpdateSortingLayer();
	void ManageInput();
	void FilterMovementDirectionBasedOnCollisionData(Vector2& moveDirection);
	void LightAttack();
	void PerformDodge();
	void PlayRunOrIdleAnimation() const;
	bool OnContactBegin(const cocos2d::PhysicsBody* otherBody);
	bool OnContactEnd(const cocos2d::PhysicsBody* otherBody);
	bool OnSortingLayerContactBegin(const cocos2d::PhysicsBody* otherBody);	
	void ResetCollisionData();
	void SetCollisionData(cocos2d::Node* otherNode);

private:
	static const String			s_eventOnPlayerHealthChanged;
	static const String			s_eventOnPlayerStaminaChanged;
	AttackComponent*			m_attackComponent;
	Vector2						m_lastValidMoveDirection;

	bool					m_isDodging;
	bool					m_isAttackComboDelayExpired;
	float					m_dodgeSpeed;
	float					m_dodgeTime;
	float					m_timeBetweenComboInput;
	float					m_dodgeStaminaConsumption;
	long					m_lastTimePerformedLightAttack;
	int						m_curAttackAnimId;
	const int				m_lastAttackAnimId;
	const int				m_firstAttackAnimId;
	cocos2d::Node*			m_lastCollisionNode;
	
	bool					m_isCollidedFromLeft;
	bool					m_isCollidedFromRight;
	bool					m_isCollidedFromTop;
	bool					m_isCollidedFromBottom;
};

NS_LIGHTSOULS_END
