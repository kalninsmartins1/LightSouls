#pragma once

#include "2d/CCSprite.h"
#include "World/Entity/Entity.h"
#include "World/Entity/Components/Attack/LongSwordAttackComponent.h"

class AnimComponent;
class GameInput;

class Player: public Entity
{
public:
	Player();

public:
	static const String& GetEventOnHealthChanged();
	static const String& GetEventOnStaminaChanged();
	static const String& GetEventOnGiveDamage();
	static const String& GetEventOnPlayerDisappeared();
	virtual EntityType	 GetEntityType() const override;

	void SetDodgeStaminaConsumption(float dodgeStaminaConumption);
	void SetDodgeSpeed(float dodgeSpeed);
	void SetDodgeTime(float dodgeTime);

	static Player*	Create(const String& pathToXML);
	bool			Init(const String& pathToXML);
	
	virtual void	Update(float deltaTime) override;

protected:
	virtual void DispatchOnHealthReduceEvent() override;
	virtual void DispatchOnStaminaChangedEvent() const override;
	virtual void DispatchOnGiveDamageEvent() const override;	
	virtual void DispatchOnDisappeared() const override;

private:
	void SetCollisionData(cocos2d::Node* otherNode);

	void StartDodging();
	void StopDodging();
	void OnDodgeFinished();

	void ManageInput();
	void FilterMovementDirectionBasedOnCollisionData(Vector2& moveDirection);
	void Attack(GenericAttackComponent& attackComponent);
	void PerformDodge();
	void PlayRunOrIdleAnimation() const;
	bool OnContactBegin(const Vector2& contactPoint, const cocos2d::PhysicsBody* otherBody);
	bool OnContactEnd(const Vector2& contactPoint, const cocos2d::PhysicsBody* otherBody);
	bool OnProjectileHit(const Vector2& contactPoint, const cocos2d::PhysicsBody* otherBody);
	void ResetCollisionData();	

private:
	static const String			s_eventOnPlayerHealthChanged;
	static const String			s_eventOnPlayerStaminaChanged;
	static const String			s_eventOnPlayerGiveDamage;
	static const String			s_eventOnPlayerDodged;
	static const String			s_eventOnPlayerDisappeared;
	GenericAttackComponent*		m_attackComponent;
	Vector2						m_lastValidMoveDirection;

	bool					m_isDodging;
	float					m_dodgeSpeed;
	float					m_dodgeTime;
	float					m_timeBetweenComboInput;
	float					m_dodgeStaminaConsumption;
	
	bool					m_isCollidedFromLeft;
	bool					m_isCollidedFromRight;
	bool					m_isCollidedFromTop;
	bool					m_isCollidedFromBottom;
};


