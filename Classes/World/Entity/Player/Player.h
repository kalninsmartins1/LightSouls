#pragma once
#include "2d/CCSprite.h"
#include "World/Entity/Entity.h"
#include "World/Entity/Components/LongSwordAttackComponent.h"

class PlayerAnimComponent;
class GameInput;

class Player: public Entity
{

public:
	static Player* create(const std::string& pPathToXML);
	Player();
	bool init(const std::string& pathToXML);
	virtual void update(float deltaTime) override;

	void setTimeBetweenComboInput(float timeBetweenComboInput);

private:
	void onDodgeFinished();
	void onAttackFinished();
	void onLightAttackComboExpired();
	
	void manageInput();
	void lightAttack();
	void performDodge();
	void playRunOrIdleAnimation() const;
	void onContactBegin(const cocos2d::PhysicsBody* otherBody);

	// Returns negative value if time for combo has expired
	float getSecondsForValidLighAttackCombo() const;

	PlayerAnimComponent* m_pAnimComponent;
	AttackComponent* m_pAttackComponent;
	Vector2 m_lastValidMoveDirection;

	bool m_bIsAttackComboDelayExpired;
	float m_timeBetweenComboInput;
	long m_lastTimePerformedLightAttack;
	unsigned short int m_curLightAttackAnimIdx;
};
