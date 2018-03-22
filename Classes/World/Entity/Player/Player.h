#pragma once

#include "2d/CCSprite.h"
#include "World/Entity/Entity.h"
#include "World/Entity/Components/LongSwordAttackComponent.h"

class PlayerAnimComponent;
class GameInput;

class Player: public Entity
{

public:
	Player();

	void SetTimeBetweenComboInput(float timeBetweenComboInput);
	
	static Player*	Create(const std::string& pPathToXML);
	bool			Init(const std::string& pathToXML);
	
	virtual void update(float deltaTime) override;


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

	PlayerAnimComponent*	m_animComponent;
	AttackComponent*		m_attackComponent;
	Vector2					m_lastValidMoveDirection;

	bool				m_bIsAttackComboDelayExpired;
	float				m_timeBetweenComboInput;
	long				m_lastTimePerformedLightAttack;
	unsigned short int	m_curLightAttackAnimIdx;
};
