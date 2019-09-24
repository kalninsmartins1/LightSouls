#pragma once

#include "World/Entity/Entity.h"

class AnimComponent;
class GameInput;
class LongSwordAttackComponent;
class GenericAttackComponent;
class Cursor;

class Player: public Entity
{
public:
	Player();

public:
	static const char*		GetEventOnHealthChanged();
	static const char*		GetEventOnStaminaChanged();
	static const char*		GetEventOnGiveDamage();
	static const char*		GetEventOnPlayerDisappeared();

	void SetDodgeStaminaConsumption(float dodgeStaminaConumption);
	void SetDodgeSpeed(float dodgeSpeed);
	void SetDodgeTime(float dodgeTime);

	static Player*	Create(const String& pathToXML);
	bool			Init(const String& pathToXML);
	bool			InitCursor(const String& pathToXML);
	virtual void	Update(float deltaTime) override;
	virtual void	setParent(Node* parent) override;

protected:
	virtual void DispatchOnHealthReduceEvent() override;
	virtual void DispatchOnStaminaChangedEvent() const override;
	virtual void DispatchOnGiveDamageEvent() const override;	
	virtual void DispatchOnDisappeared() const override;

private:
	const char* GetDodgeEventForDispatch() const;
	void SetCollisionData(cc::Node* otherNode);

	void StartDodging();
	void StopDodging();
	void OnDodgeFinished();

	void ManageInput();
	void FilterMovementDirectionBasedOnCollisionData(Vector2& moveDirection);
	void Attack(GenericAttackComponent& attackComponent);
	void PerformDodge();
	void PlayRunOrIdleAnimation() const;
	void ResetCollisionData();

	bool OnContactBegin(const Vector2& contactPoint, const cc::PhysicsBody* otherBody);
	bool OnContactEnd(const Vector2& contactPoint, const cc::PhysicsBody* otherBody);
	bool OnProjectileHit(const Vector2& contactPoint, const cc::PhysicsBody* otherBody);
	bool InitAttackComponent();

private:
	static const char*			s_eventOnPlayerHealthChanged;
	static const char*			s_eventOnPlayerStaminaChanged;
	static const char*			s_eventOnPlayerGiveDamage;
	static const char*			s_eventOnPlayerDodgedHorizontally;
	static const char*			s_eventOnPlayerDodgedUp;
	static const char*			s_eventOnPlayerDodgedDown;
	static const char*			s_eventOnPlayerDisappeared;
	LongSwordAttackComponent*	m_attackComponent;
	Vector2						m_lastValidMoveDirection;
	Cursor*						m_cursor;

	bool	m_isDodging;
	float	m_dodgeSpeed;
	float	m_dodgeTime;
	float	m_timeBetweenComboInput;
	float	m_dodgeStaminaConsumption;
	
	bool	m_isCollidedFromLeft;
	bool	m_isCollidedFromRight;
	bool	m_isCollidedFromTop;
	bool	m_isCollidedFromBottom;
};


